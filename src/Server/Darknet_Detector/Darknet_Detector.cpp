#include "Darknet_Detector/Darknet_Detector.h"
#include "Darknet_Detector/darknet/Option_Config.h"

Darknet_Detector::Darknet_Detector()
{
    detector_loaded_ = false;
}

Darknet_Detector::~Darknet_Detector()
{

}

void Darknet_Detector::init_model(string cfg_file, string weights_file)
{
    if(detector_loaded_)
    {
        cout << "Darknet_Detector::init_model : detector loaded.";
        return;
    }

    char *cfgfile = new char[cfg_file.length() + 1];
    strcpy(cfgfile, cfg_file.c_str());
    char *weightsfile = new char[weights_file.length() + 1];
    strcpy(weightsfile, weights_file.c_str());

    net_ = load_network(cfgfile, weightsfile, 0);
    set_batch_network(net_, 1);
    srand(2222222);

    thresh_ = 0.5;
    hier_thresh_ = 0.5;
    nms_ = 0.45;

    detector_loaded_ = true;
}

//void Darknet_Detector::slot_LoadVideo(string video_path)
//{
//    capture_.open(video_path.toStdString());
//    if (!capture_.isOpened())
//    {
//        printf("文件打开失败");
//    }
//    Mat frame;

//    while (true)
//    {
//        capture_ >> frame;
//        vector<bbox_t> result_vec = darknet_detector_->detect(frame);
//        draw_boxes(frame, result_vec, obj_names_);

//        namedWindow("test", CV_WINDOW_NORMAL);
//        imshow("test", frame);
//        waitKey(3);
//    }
//}

bool Darknet_Detector::detect_image(Mat image)
{
    return detect(image);
}

image Darknet_Detector::ipl_to_image(IplImage* src)
{
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    int step = src->widthStep;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    return im;
}

bool Darknet_Detector::detect(Mat input)
{
    double time;

    input_ = input;
    IplImage ipl = input_;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);

    image sized = letterbox_image(im, net_->w, net_->h);
    //image sized = resize_image(im, net->w, net->h);
    //image sized2 = resize_max(im, net->w);
    //image sized = crop_image(sized2, -((net->w - sized2.w)/2), -((net->h - sized2.h)/2), net->w, net->h);
    //resize_network(net, sized.w, sized.h);
    layer l = net_->layers[net_->n-1];


    float *X = sized.data;
    time=what_time_is_it_now();
    network_predict(net_, X);
    //printf("Predicted in %f seconds.\n", what_time_is_it_now()-time);
    int nboxes = 0;
    detection *dets = get_network_boxes(net_, im.w, im.h, thresh_, hier_thresh_, 0, 1, &nboxes);
    //if (nms) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);
    if (nms_) do_nms_sort(dets, nboxes, l.classes, nms_);

    result_.clear();
    result_trans_.clear();

    for(int i = 0; i < nboxes; ++i)
    {
        char labelstr[4096] = {0};
        int iclass = -1;
        for(int j = 0; j < l.classes; ++j)
        {
            if (dets[i].prob[j] > thresh_)
            {
                if (iclass < 0)
                {
                    strcat(labelstr, to_string(j).c_str());
                    iclass = j;
                }
                else
                {
                    strcat(labelstr, ", ");
                    strcat(labelstr, to_string(j).c_str());
                }
                //printf("%s: %.0f%%\n", to_string(j).c_str(), dets[i].prob[j]*100);
            }
        }
        if(iclass >= 0)
        {
            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            int offset = iclass*123457 % l.classes;
            float red = get_color(2,offset,l.classes);
            float green = get_color(1,offset,l.classes);
            float blue = get_color(0,offset,l.classes);
            float rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            vector<double> bbox;
            bbox.resize(5);
            bbox[0] = (b.x-b.w/2.);
            bbox[1] = (b.y-b.h/2.);
            bbox[2] = (b.x+b.w/2.);
            bbox[3] = (b.y+b.h/2.);
            bbox[4] = iclass;

            result_.emplace_back(bbox);

            int left  = (b.x-b.w/2.)*im.w;
            int right = (b.x+b.w/2.)*im.w;
            int top   = (b.y-b.h/2.)*im.h;
            int bot   = (b.y+b.h/2.)*im.h;

            if(left < 0) left = 0;
            if(right > im.w-1) right = im.w-1;
            if(top < 0) top = 0;
            if(bot > im.h-1) bot = im.h-1;

            vector<int> trans;
            trans.emplace_back(left);
            trans.emplace_back(top);
            trans.emplace_back(right);
            trans.emplace_back(bot);
            trans.emplace_back(iclass);

            result_trans_.emplace_back(trans);
        }
    }

    free_detections(dets, nboxes);

    free_image(im);
    free_image(sized);

    return true;
}
