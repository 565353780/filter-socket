import json
import cv2
import numpy as np
from math import cos, sin, pi
import os
import codecs
from time import time
import random
import shutil

img_type = '.jpg'
process_index = 6
process_folder_list = os.listdir(os.getcwd() + "/resnet-test/sand-pic/")
dataset_path = os.getcwd() + "/resnet-test/sand-pic/" + process_folder_list[process_index] + "/"
bright_value_list = [0]
contrast_value_list = [100]
scale_dx_list = [1]
scale_dy_list = [1]
flip_axis_x_list = [False]
flip_axis_y_list = [False]
shift_dx_list = [0]
shift_dy_list = [0]
rotate_angle_list = [0]
data_multi_times = 10
resize_cut = [896, 896, 10]


# bright_value_list = [0]
# contrast_value_list = [100]
# scale_dx_list = [1, 0.75]
# scale_dy_list = [1, 0.75]
# flip_axis_x_list = [False, True]
# flip_axis_y_list = [False, True]
# shift_dx_list = [0, -1500, -750, 750, 1500]
# shift_dy_list = [0]
# rotate_angle_list = [0]

target_path = dataset_path + "/../../sand_enhance_dataset/" + str(process_index) + "/"

class SampleEnhancement:
    def __init__(self):
        self.file_name = None

        self.img = None
        self.img_copy = None
        self.img_to_show = None
        self.json = None

        self.flag_flip = False
        self.flag_shift = False
        self.flag_scale = False
        self.flag_rotate = False
        self.flag_contrast = False
        self.flag_resize_cut = False

        self.reload_img = True

    def reset(self):
        if self.reload_img:
            self.file_name = None

            self.img = None
            self.img_copy = None
            self.img_to_show = None
        else:
            self.img = self.img_copy.copy()

            self.reload_img = True

        self.json = None

        self.flag_flip = False
        self.flag_shift = False
        self.flag_scale = False
        self.flag_rotate = False
        self.flag_contrast = False

    def load_img(self, img_path):
        img_name = None

        if '/' in img_path:
            img_name = img_path.split('/')[len(img_path.split('/')) - 1]
        else:
            img_name = img_path

        if '.jpg' in img_name:
            img_name = img_name.split('.jpg')[0]

        if img_name == self.file_name:
            self.reload_img = False

            self.reset()
        else:
            self.reset()

            if '/' in img_path:
                self.file_name = img_path.split('/')[len(img_path.split('/')) - 1]
            else:
                self.file_name = img_path

            if '.jpg' in self.file_name:
                self.file_name = self.file_name.split('.jpg')[0]

            self.img = cv2.imread(img_path)
            self.img_copy = self.img.copy()
            self.img_to_show = self.img.copy()

        self.load_json(target_path + self.file_name + '.json')

    def load_json(self, json_path):
        if not os.path.exists(json_path):
            return

        with open(json_path, 'r') as f:
            self.json = json.load(f)

    def contrast(self, contrast_value=100, bright_value=0):
        if contrast_value == 100 and bright_value == 0:
            return False

        self.img = cv2.addWeighted(self.img, 0.01 * contrast_value,
                                   np.zeros((self.img.shape[0], self.img.shape[1], self.img.shape[2]), self.img.dtype),
                                   0, bright_value)

        return True

    def flip(self, axis_x=False, axis_y=False, check_bbox=True):
        if not axis_x and not axis_y:
            return False

        DX = self.json['Area']['shape'][0]
        DY = self.json['Area']['shape'][1]

        M = None

        if axis_x:
            if axis_y:
                M = np.float32([[-1, 0, DX], [0, -1, DY]])
            else:
                M = np.float32([[1, 0, 0], [0, -1, DY]])
        elif axis_y:
            M = np.float32([[-1, 0, DX], [0, 1, 0]])

        self.img = cv2.warpAffine(self.img, M, (self.img.shape[1], self.img.shape[0]))

        new_polygons = []
        label_used = []

        for polygon in self.json['Area']['polygons']:
            x1 = polygon[0][0]
            y1 = polygon[0][1]
            x2 = polygon[2][0]
            y2 = polygon[2][1]

            if axis_x:
                if axis_y:
                    x1 = -x1 + DX
                    y1 = -y1 + DY
                    x2 = -x2 + DX
                    y2 = -y2 + DY
                else:
                    y1 = -y1 + DY
                    y2 = -y2 + DY
            elif axis_y:
                x1 = -x1 + DX
                x2 = -x2 + DX

            if check_bbox:
                if x2 <= 0 or y2 <= 0 or x1 >= self.json['Area']['shape'][0] - 1 or y1 >= self.json['Area']['shape'][
                    1] - 1:
                    label_used.append(0)
                    continue

                if x1 < 0:
                    x1 = 0
                if y1 < 0:
                    y1 = 0
                if x2 > self.json['Area']['shape'][0] - 1:
                    x2 = self.json['Area']['shape'][0] - 1
                if y2 > self.json['Area']['shape'][1] - 1:
                    y2 = self.json['Area']['shape'][1] - 1

            label_used.append(1)
            new_polygons.append(
                [[int(x1), int(y1)], [int(x1), int(y2)], [int(x2), int(y2)], [int(x2), int(y1)], [int(x1), int(y1)]])

        if 0 in label_used:
            new_labels = []
            for i in range(len(label_used)):
                if label_used[i] == 1:
                    new_labels.append(self.json['Area']['labels'][i])

            self.json['Area']['labels'] = new_labels

        self.json['Area']['polygons'] = new_polygons

        return True

    def scale(self, dx=1, dy=1, check_bbox=True):
        if dx <= 0 or dy <= 0 or (dx == 1 and dy == 1):
            return False

        DX = (1 - dx) * self.json['Area']['shape'][0] / 2
        DY = (1 - dy) * self.json['Area']['shape'][1] / 2

        M = np.float32([[dx, 0, DX], [0, dy, DY]])

        self.img = cv2.warpAffine(self.img, M, (self.img.shape[1], self.img.shape[0]))

        new_polygons = []
        label_used = []

        for polygon in self.json['Area']['polygons']:
            x1 = polygon[0][0] * dx + DX
            y1 = polygon[0][1] * dy + DY
            x2 = polygon[2][0] * dx + DX
            y2 = polygon[2][1] * dy + DY

            if check_bbox:
                if x2 <= 0 or y2 <= 0 or x1 >= self.json['Area']['shape'][0] - 1 or y1 >= self.json['Area']['shape'][
                    1] - 1:
                    label_used.append(0)
                    continue

                if x1 < 0:
                    x1 = 0
                if y1 < 0:
                    y1 = 0
                if x2 > self.json['Area']['shape'][0] - 1:
                    x2 = self.json['Area']['shape'][0] - 1
                if y2 > self.json['Area']['shape'][1] - 1:
                    y2 = self.json['Area']['shape'][1] - 1

            label_used.append(1)
            new_polygons.append(
                [[int(x1), int(y1)], [int(x1), int(y2)], [int(x2), int(y2)], [int(x2), int(y1)], [int(x1), int(y1)]])

        if 0 in label_used:
            new_labels = []
            for i in range(len(label_used)):
                if label_used[i] == 1:
                    new_labels.append(self.json['Area']['labels'][i])

            self.json['Area']['labels'] = new_labels

        self.json['Area']['polygons'] = new_polygons

        return True

    def rotate(self, angle=0, check_bbox=True):
        if angle == 0:
            return False

        alpah = angle * pi / 180

        cos_alpha = cos(alpah)
        sin_alpha = sin(alpah)

        DX = (-self.json['Area']['shape'][0] * cos_alpha + self.json['Area']['shape'][1] * sin_alpha +
              self.json['Area']['shape'][0]) / 2
        DY = (-self.json['Area']['shape'][0] * sin_alpha - self.json['Area']['shape'][1] * cos_alpha +
              self.json['Area']['shape'][1]) / 2

        M = np.float32([[cos_alpha, -sin_alpha, DX], [sin_alpha, cos_alpha, DY]])

        self.img = cv2.warpAffine(self.img, M, (self.img.shape[1], self.img.shape[0]))

        new_polygons = []
        label_used = []

        print(self.json['Area']['shape'])

        for polygon in self.json['Area']['polygons']:
            x1_temp = polygon[0][0] * cos_alpha - polygon[0][1] * sin_alpha + DX
            y1_temp = polygon[0][0] * sin_alpha + polygon[0][1] * cos_alpha + DY
            x2_temp = polygon[1][0] * cos_alpha - polygon[1][1] * sin_alpha + DX
            y2_temp = polygon[1][0] * sin_alpha + polygon[1][1] * cos_alpha + DY
            x3_temp = polygon[2][0] * cos_alpha - polygon[2][1] * sin_alpha + DX
            y3_temp = polygon[2][0] * sin_alpha + polygon[2][1] * cos_alpha + DY
            x4_temp = polygon[3][0] * cos_alpha - polygon[3][1] * sin_alpha + DX
            y4_temp = polygon[3][0] * sin_alpha + polygon[3][1] * cos_alpha + DY

            x1 = min(x1_temp, x2_temp, x3_temp, x4_temp)
            y1 = min(y1_temp, y2_temp, y3_temp, y4_temp)
            x2 = max(x1_temp, x2_temp, x3_temp, x4_temp)
            y2 = max(y1_temp, y2_temp, y3_temp, y4_temp)

            if check_bbox:
                if x2 <= 0 or y2 <= 0 or x1 >= self.json['Area']['shape'][0] - 1 or y1 >= self.json['Area']['shape'][
                    1] - 1:
                    label_used.append(0)
                    continue

                if x1 < 0:
                    x1 = 0
                if y1 < 0:
                    y1 = 0
                if x2 > self.json['Area']['shape'][0] - 1:
                    x2 = self.json['Area']['shape'][0] - 1
                if y2 > self.json['Area']['shape'][1] - 1:
                    y2 = self.json['Area']['shape'][1] - 1

            label_used.append(1)
            new_polygons.append(
                [[int(x1), int(y1)], [int(x1), int(y2)], [int(x2), int(y2)], [int(x2), int(y1)], [int(x1), int(y1)]])

        if 0 in label_used:
            new_labels = []
            for i in range(len(label_used)):
                if label_used[i] == 1:
                    new_labels.append(self.json['Area']['labels'][i])

            self.json['Area']['labels'] = new_labels

        self.json['Area']['polygons'] = new_polygons

        return True

    def shift(self, dx=0, dy=0, check_bbox=True):
        if dx == 0 and dy == 0:
            return False

        M = np.float32([[1, 0, dx], [0, 1, dy]])

        self.img = cv2.warpAffine(self.img, M, (self.img.shape[1], self.img.shape[0]))

        new_polygons = []
        label_used = []

        for polygon in self.json['Area']['polygons']:
            x1 = polygon[0][0] + dx
            y1 = polygon[0][1] + dy
            x2 = polygon[2][0] + dx
            y2 = polygon[2][1] + dy

            if check_bbox:
                if x2 <= 0 or y2 <= 0 or x1 >= self.json['Area']['shape'][0] - 1 or y1 >= self.json['Area']['shape'][
                    1] - 1:
                    label_used.append(0)
                    continue

                if x1 < 0:
                    x1 = 0
                if y1 < 0:
                    y1 = 0
                if x2 > self.json['Area']['shape'][0] - 1:
                    x2 = self.json['Area']['shape'][0] - 1
                if y2 > self.json['Area']['shape'][1] - 1:
                    y2 = self.json['Area']['shape'][1] - 1

            label_used.append(1)
            new_polygons.append(
                [[int(x1), int(y1)], [int(x1), int(y2)], [int(x2), int(y2)], [int(x2), int(y1)], [int(x1), int(y1)]])

        if 0 in label_used:
            new_labels = []
            for i in range(len(label_used)):
                if label_used[i] == 1:
                    new_labels.append(self.json['Area']['labels'][i])

            self.json['Area']['labels'] = new_labels

        self.json['Area']['polygons'] = new_polygons

        return True

    def constrast_flip_scale_rotate_shift(self, contrast_value=100, bright_value=0, flip_axis_x=False,
                                          flip_axis_y=False, scale_dx=1, scale_dy=1, rotate_angle=0, shift_dx=0,
                                          shift_dy=0, resize_cut=[-1, -1, -1, -1], check_bbox=True):
        if (not flip_axis_x and not flip_axis_y) and (contrast_value == 100 and bright_value == 0) and (
                scale_dx <= 0 or scale_dy <= 0 or (scale_dx == 1 and scale_dy == 1)) and rotate_angle == 0 and (
                shift_dx == 0 and shift_dy == 0) and resize_cut[2] < 1:
            return
        
        img_width = self.img.shape[0]
        img_height = self.img.shape[1]
        img_formart = self.img.shape[2]

        self.img = cv2.addWeighted(self.img, 0.01 * contrast_value,
                                   np.zeros((img_width, img_height, img_formart), self.img.dtype),
                                   0, bright_value)

        flip_DX = img_width
        flip_DY = img_height

        M_flip = None

        if flip_axis_x:
            if flip_axis_y:
                M_flip = np.float32([[-1, 0, flip_DX], [0, -1, flip_DY], [0, 0, 1]])
            else:
                M_flip = np.float32([[1, 0, 0], [0, -1, flip_DY], [0, 0, 1]])
        elif flip_axis_y:
            M_flip = np.float32([[-1, 0, flip_DX], [0, 1, 0], [0, 0, 1]])
        else:
            M_flip = np.float32([[1, 0, 0], [0, 1, 0], [0, 0, 1]])

        scale_DX = (1 - scale_dx) * img_width / 2
        scale_DY = (1 - scale_dy) * img_height / 2

        M_scale = np.float32([[scale_dx, 0, scale_DX], [0, scale_dy, scale_DY], [0, 0, 1]])

        alpah = rotate_angle * pi / 180

        cos_alpha = cos(alpah)
        sin_alpha = sin(alpah)

        rotate_DX = (-img_width * cos_alpha + img_height * sin_alpha +
                     img_width) / 2
        rotate_DY = (-img_width * sin_alpha - img_height * cos_alpha +
                     img_height) / 2

        M_rotate = np.float32([[cos_alpha, -sin_alpha, rotate_DX], [sin_alpha, cos_alpha, rotate_DY], [0, 0, 1]])

        M_shift = np.float32([[1, 0, shift_dx], [0, 1, shift_dy], [0, 0, 1]])

        M = np.dot(M_shift, np.dot(M_rotate, np.dot(M_scale, M_flip)))

        M_copy = M[:2]

        self.img = cv2.warpAffine(self.img, M_copy, (self.img.shape[1], self.img.shape[0]))

        if resize_cut[2] > 0:
            start_x = random.randint(0, img_width - resize_cut[0])
            start_y = random.randint(0, img_height - resize_cut[1])
            self.img = self.img[start_x:start_x+resize_cut[0], start_y:start_y+resize_cut[1]]

        new_polygons = []
        label_used = []

        if self.json != None:
            for polygon in self.json['Area']['polygons']:
                point1 = [polygon[0][0], polygon[0][1], 1]
                point2 = [polygon[1][0], polygon[1][1], 1]
                point3 = [polygon[2][0], polygon[2][1], 1]
                point4 = [polygon[3][0], polygon[3][1], 1]

                x1_temp = np.dot(M, point1)[0]
                y1_temp = np.dot(M, point1)[1]
                x2_temp = np.dot(M, point2)[0]
                y2_temp = np.dot(M, point2)[1]
                x3_temp = np.dot(M, point3)[0]
                y3_temp = np.dot(M, point3)[1]
                x4_temp = np.dot(M, point4)[0]
                y4_temp = np.dot(M, point4)[1]

                x1 = min(x1_temp, x2_temp, x3_temp, x4_temp)
                y1 = min(y1_temp, y2_temp, y3_temp, y4_temp)
                x2 = max(x1_temp, x2_temp, x3_temp, x4_temp)
                y2 = max(y1_temp, y2_temp, y3_temp, y4_temp)

                if check_bbox:
                    if x2 <= 0 or y2 <= 0 or x1 >= img_width - 1 or y1 >= self.json['Area']['shape'][
                        1] - 1:
                        label_used.append(0)
                        continue

                    if x1 < 0:
                        x1 = 0
                    if y1 < 0:
                        y1 = 0
                    if x2 > img_width - 1:
                        x2 = img_width - 1
                    if y2 > img_height - 1:
                        y2 = img_height - 1

                label_used.append(1)
                new_polygons.append(
                    [[int(x1), int(y1)], [int(x1), int(y2)], [int(x2), int(y2)], [int(x2), int(y1)], [int(x1), int(y1)]])

            if 0 in label_used:
                new_labels = []
                for i in range(len(label_used)):
                    if label_used[i] == 1:
                        new_labels.append(self.json['Area']['labels'][i])

                self.json['Area']['labels'] = new_labels

            self.json['Area']['polygons'] = new_polygons

        if flip_axis_x or flip_axis_y:
            self.flag_flip = True

        if contrast_value != 100 or bright_value != 0:
            self.flag_contrast = True

        if scale_dx > 0 and scale_dy > 0 and (scale_dx != 1 or scale_dy != 1):
            self.flag_scale = True

        if rotate_angle != 0:
            self.flag_rotate = True

        if shift_dx != 0 or shift_dy != 0:
            self.flag_shift = True
        
        if resize_cut[2] > 0:
            self.flag_resize_cut = True

    def trans(self, img_path, flip_axis_x=False, flip_axis_y=False, shift_dx=0, shift_dy=0, scale_dx=1, scale_dy=1,
              rotate_angle=0, contrast_value=100, bright_value=0, resize_cut=[-1, -1, -1, -1], check_bbox=True):
        self.load_img(dataset_path + img_path)

        self.constrast_flip_scale_rotate_shift(contrast_value, bright_value, flip_axis_x, flip_axis_y, scale_dx,
                                               scale_dy, rotate_angle, shift_dx, shift_dy, resize_cut, check_bbox)

        output_file_name = self.file_name

        if self.flag_flip:
            if flip_axis_x and flip_axis_y:
                output_file_name += '_flip_xy'
            elif flip_axis_x:
                output_file_name += '_flip_x'
            else:
                output_file_name += '_flip_y'

        if self.flag_shift:
            if shift_dx != 0:
                output_file_name += '_shift_x_' + str(shift_dx)
            if shift_dy != 0:
                output_file_name += '_shift_y_' + str(shift_dy)

        if self.flag_scale:
            if scale_dx != 1:
                output_file_name += '_scale_x_' + str(scale_dx)
            if scale_dy != 1:
                output_file_name += '_scale_y_' + str(scale_dy)

        if self.flag_rotate:
            output_file_name += '_rotate_' + str(rotate_angle)

        if self.flag_contrast:
            if contrast_value != 100:
                output_file_name += '_constrast_' + str(contrast_value)
            if bright_value != 0:
                output_file_name += '_bright_' + str(bright_value)
        
        if self.flag_resize_cut:
            output_file_name += '_resize_cut_[' + str(resize_cut[0]) + "_" + str(resize_cut[1]) + "]x" + str(resize_cut[3])

        if output_file_name == self.file_name:
            return

        cv2.imwrite(target_path + output_file_name + '.jpg', self.img)

        # label_num = len(self.json['Area']['labels'])
        # if label_num != len(self.json['Area']['track']['Array']):
        #     new_array = []
        #     for i in range(label_num):
        #         new_array.append(i + 1)

        #     self.json['Area']['track']['Array'] = new_array

        #     self.json['Area']['track']['NextId'] = label_num

        if self.json != None:
            with codecs.open(target_path + output_file_name + '.json', 'w', 'utf-8') as outf:
                json.dump(self.json, outf, ensure_ascii=False)
                outf.write('\n')

    def draw_bbox(self):
        self.img_to_show = self.img.copy()

        if self.json != None:
            for polygon in self.json['Area']['polygons']:
                for i in range(4):
                    cv2.line(self.img_to_show, (polygon[i][0], polygon[i][1]), (polygon[i + 1][0], polygon[i + 1][1]),
                            (0, 255, 0), 3)

    def show_img(self):
        self.draw_bbox()

        cv2.imshow('image', self.img_to_show)

        cv2.waitKey()


def Main_Sample_Enhancement():
    sample_enhancement = SampleEnhancement()
    
    if os.path.exists(target_path):
        shutil.rmtree(target_path[:-1])
    
    os.makedirs(target_path)
    
    # shutil.copytree(dataset_path, target_path[:-1])

    file_list = os.listdir(dataset_path)

    img_list = []

    if img_type != '.jpg':
        for file_name in file_list:
            if file_name[-4:] == img_type:
                temp_img = cv2.imread(target_path + file_name)
                cv2.imwrite(target_path + file_name[:-4] + '.jpg', temp_img)
                os.remove(target_path + file_name)

        file_list = os.listdir(target_path)

    for file_name in file_list:
        if file_name[-4:] == '.jpg' and "train" not in file_name:
            img_list.append(file_name)

    total_file_num = len(bright_value_list) * len(contrast_value_list) * len(scale_dx_list) * len(scale_dy_list) * len(
        flip_axis_x_list) * len(flip_axis_y_list) * len(shift_dx_list) * len(shift_dy_list) * len(rotate_angle_list) * len(
        img_list) * resize_cut[2]
    
    if data_multi_times < 1:
        return

    rand_max = int(total_file_num / len(img_list) / data_multi_times) - 1

    print('Target dataset size : ' + str(len(img_list) * data_multi_times))
    print('Start create new dataset : Choose 1 image from each ' + str(rand_max + 1) + ' images randomly')

    saved_num = 0

    start = time()

    if rand_max == 0:
        rand_max = 1

    for img_name in img_list:
        for bright_value in bright_value_list:
            for contrast_value in contrast_value_list:
                for scale_dx in scale_dx_list:
                    for scale_dy in scale_dy_list:
                        for flip_axis_x in flip_axis_x_list:
                            for flip_axis_y in flip_axis_y_list:
                                for shift_dx in shift_dx_list:
                                    for shift_dy in shift_dy_list:
                                        for rotate_angle in rotate_angle_list:
                                            for cut_index in range(resize_cut[2]):
                                                saved_num += 1
                                                if random.randint(0, rand_max) == 0:
                                                    sample_enhancement.trans(img_name,
                                                                            flip_axis_x=flip_axis_x, flip_axis_y=flip_axis_y,
                                                                            shift_dx=shift_dx, shift_dy=shift_dy,
                                                                            scale_dx=scale_dx, scale_dy=scale_dy,
                                                                            rotate_angle=rotate_angle,
                                                                            contrast_value=contrast_value,
                                                                            bright_value=bright_value,
                                                                            resize_cut=[resize_cut[0], resize_cut[1], resize_cut[2], cut_index])

                                                    # sample_enhancement.show_img()

                                                print('\rSaved : ' + str(saved_num) + ' / ' + str(total_file_num) + '\t' + str(
                                                    format(100 * saved_num / total_file_num, '.4f')) + '%', end='')


    print('')

    print(time() - start)

    return

if __name__ == "__main__":
    Main_Sample_Enhancement()
