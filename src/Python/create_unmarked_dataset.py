import os
import shutil

work_path = "Z:/chLi/tiedaobu/2C数据/3.16(2C)/"

target_path = "Z:/chLi/tiedaobu/2c_boundary/"

railway_line_folder_list = os.listdir(work_path)

select_pic_num_from_each_folder = 10

image_folder_path_list = []

for railway_line_folder in railway_line_folder_list:

    if "valid" not in railway_line_folder and "config" not in railway_line_folder:

        current_global_local_folder_path = work_path + railway_line_folder + "/"

        if os.path.isdir(current_global_local_folder_path):

            global_local_folder_path_list = os.listdir(current_global_local_folder_path)

            for global_local_folder in global_local_folder_path_list:

                current_index_folder_path = current_global_local_folder_path + global_local_folder + "/"

                if os.path.isdir(current_index_folder_path):

                    current_image_folder_list = os.listdir(current_index_folder_path)

                    for current_image_folder in current_image_folder_list:

                        current_image_folder_path = current_index_folder_path + current_image_folder + "/"

                        if os.path.isdir(current_image_folder_path):

                            image_folder_path_list.append(current_image_folder_path)

scanned_folder_num = 0

for image_folder in image_folder_path_list:

    image_name_list = os.listdir(image_folder)

    copyed_image_num = 0

    for image_name in image_name_list:

        if ".jpg" in image_name:

            shutil.copy(image_folder  + image_name, target_path + str(scanned_folder_num) + "_" + str(copyed_image_num) + ".jpg")

            copyed_image_num += 1
        
        if copyed_image_num > select_pic_num_from_each_folder:
            break
    
    scanned_folder_num += 1