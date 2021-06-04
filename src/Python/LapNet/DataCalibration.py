import cv2
import os
import numpy as np
from math import sqrt


class DataCalibration:
    def __init__(self, target_path, max_width, max_height, mask_type):
        self.target_path = target_path
        self.max_width = max_width
        self.max_height = max_height
        self.mask_type = mask_type
        self.rectangle = []
        self.rects = []
        self.picture_list = []
        self.resize_scale = 1
        self.current_img_name = ''
        self.source_image = None
        self.image = None
        self.image_copy = None
        self.width_ = -1
        self.height_ = -1
        self.draw_closed_rects = False

        self.circle_radius = 3
        self.circle_thickness = 1
        self.line_thickness = 2

        if not os.path.exists(self.target_path):
            os.makedirs(self.target_path)
        
        if not os.path.exists(self.target_path + "point/"):
            os.makedirs(self.target_path + "point/")

    def get_listname(self, path=os.getcwd()):
        file_list = os.listdir(path)
        for i in file_list:
            if i[-3:] == "jpg":
                self.picture_list.append(i)
        return self.picture_list

    def load_image(self, image_name):
        self.source_image = cv2.imread(image_name, 1)
        self.current_img_name = image_name.split("/")[len(image_name.split("/")) - 1]
        self.image = self.source_image.copy()
        self.height_, self.width_, _ = self.source_image.shape
        self.resize_scale = 1
        temp_height_scale = 1
        temp_width_scale = 1
        if self.height_ > self.max_height:
            self.resize_scale = self.max_height / self.height_
        if self.width_ > self.max_width:
            temp_width_scale = self.max_width / self.width_
        if temp_height_scale < self.resize_scale:
            self.resize_scale = temp_height_scale
        if temp_width_scale < self.resize_scale:
            self.resize_scale = temp_width_scale
        self.image = cv2.resize(self.image,
                                (int(self.width_ * self.resize_scale), int(self.height_ * self.resize_scale)))
        self.height_, self.width_, _ = self.image.shape
        self.image_copy = self.image.copy()
    
    def change_draw_mode(self):
        if self.draw_closed_rects:
            self.draw_closed_rects = False
        else:
            self.draw_closed_rects = True

    def draw_circle(self, event, x, y, flags, param):
        if self.mask_type == "1":
            if event == cv2.EVENT_LBUTTONDOWN:
                self.rectangle.append(x)
                self.rectangle.append(y)
                rect = []
                point = [int(self.rectangle[0]), int(self.rectangle[1])]
                rect.append(point)
                self.rects.append(rect)
                cv2.circle(self.image, tuple(rect[0]), self.circle_radius, color=(0, 0, 255),
                        thickness=self.circle_thickness)
                cv2.imshow(self.current_img_name, self.image)
                self.rectangle = []
        elif self.mask_type == "2":
            if event == cv2.EVENT_LBUTTONDOWN:
                cv2.circle(self.image, (x, y), self.circle_radius, (0, 0, 255), self.circle_thickness)
                self.rectangle.append(x)
                self.rectangle.append(y)
            if event == cv2.EVENT_RBUTTONDOWN:
                if len(self.rectangle) > 0:
                    rect = []
                    for i in range(int(len(self.rectangle) / 2)):
                        point = [int(self.rectangle[2 * i]), int(self.rectangle[2 * i + 1])]
                        rect.append(point)
                    self.rects.append(rect)
                    draw_line_num = len(rect) - 1
                    if self.draw_closed_rects:
                        draw_line_num += 1
                    for i in range(draw_line_num):
                        cv2.line(self.image, tuple(rect[i]), tuple(rect[(i + 1) % len(rect)]), color=(0, 0, 255),
                                thickness=self.line_thickness)
                    cv2.imshow(self.current_img_name, self.image)
                    self.rectangle = []

    def pop_rects(self):
        self.rects = self.rects[:-1]
        self.image = self.image_copy.copy()
        for rect in self.rects:
            if len(rect) > 1:
                draw_line_num = len(rect) - 1
                if self.draw_closed_rects:
                    draw_line_num += 1
                for i in range(draw_line_num):
                    cv2.line(self.image, tuple(rect[i]), tuple(rect[(i + 1) % len(rect)]), color=(0, 0, 255),
                            thickness=self.line_thickness)
            elif len(rect) == 1:
                cv2.circle(self.image, tuple(rect[0]), self.circle_radius, color=(0, 0, 255),
                        thickness=self.circle_thickness)
        cv2.imshow(self.current_img_name, self.image)

    def generatePNG(self):
        image = np.zeros(tuple(self.source_image.shape), np.uint8)

        for rect in self.rects:
            if len(rect) > 1:
                draw_line_num = len(rect) - 1
                if self.draw_closed_rects:
                    draw_line_num += 1
                for i in range(draw_line_num):
                    cv2.line(image, tuple([int(pose / self.resize_scale) for pose in rect[i]]), tuple([int(pose / self.resize_scale) for pose in rect[(i + 1) % len(rect)]]), color=(255, 255, 255),
                            thickness=int(self.line_thickness / self.resize_scale))
            elif len(rect) == 1:
                cv2.circle(image, tuple([int(pose / self.resize_scale) for pose in rect[0]]), int(self.circle_radius / self.resize_scale), color=(255, 255, 255),
                        thickness=-1)
        
        rect, image = cv2.threshold(image, 20, 255, cv2.THRESH_BINARY)
        cv2.imwrite(self.target_path + self.current_img_name, self.source_image)
        cv2.imwrite(self.target_path + self.current_img_name[:-3] + "png", image)
        
        image = np.zeros(tuple(self.source_image.shape), np.uint8)
        
        for rect in self.rects:
            if len(rect) > 0:
                cv2.circle(image, tuple([int(pose / self.resize_scale) for pose in rect[0]]), int(self.circle_radius / self.resize_scale), color=(255, 255, 255),
                    thickness=-1)
                cv2.circle(image, tuple([int(pose / self.resize_scale) for pose in rect[len(rect) - 1]]), int(self.circle_radius / self.resize_scale), color=(255, 255, 255),
                    thickness=-1)

        rect, image = cv2.threshold(image, 20, 255, cv2.THRESH_BINARY)
        cv2.imwrite(self.target_path + "point/" + self.current_img_name, self.source_image)
        cv2.imwrite(self.target_path + "point/" + self.current_img_name[:-3] + "png", image)

        self.rects = []
        self.rectangle = []


if __name__ == '__main__':
    mask_folder_id = input("Mask Folder ID : ")

    # mask_type = input("Mask Type : 1.point ; 2.polyline :")

    mask_type = "2"

    if mask_type not in ["1", "2"]:
        print("ivalid mask type input.")
    else:
        source_image_path = "Z:/public/MaskStation/2c_allImage/MaskSpace/Yolov3Region/mask_station/" + mask_folder_id + "/"
        target_image_path = "Z:/public/MaskStation/2c_allImage/MaskSpace/LapNet/chLi_train_dataset_line_and_point/"

        max_width = 1900
        max_height = 1000

        datacalibration = DataCalibration(target_image_path, max_width, max_height, mask_type)
        pic_list = datacalibration.get_listname(source_image_path)

        img_num = len(pic_list)
        masked_num = 0

        for picture_name in pic_list:
            masked_num += 1

            print("\rMask process :", masked_num, "/", img_num, end="")

            if os.path.exists(target_image_path + picture_name):
                continue

            datacalibration.load_image(source_image_path + picture_name)

            cv2.namedWindow(picture_name)
            cv2.setMouseCallback(picture_name, datacalibration.draw_circle)

            while True:
                cv2.imshow(picture_name, datacalibration.image)
                key = cv2.waitKey(50) & 0xFF
                if key == ord('q'):
                    break
                elif key == ord('z') and len(datacalibration.rects) > 0:
                    datacalibration.pop_rects()
                elif key == ord('c'):
                    datacalibration.change_draw_mode()

            datacalibration.generatePNG()
            cv2.destroyAllWindows()
        
        print()
