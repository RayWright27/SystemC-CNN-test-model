import time
import numpy as np
import matplotlib.pylab as plt
import tensorflow as tf
import tensorflow_hub as hub
import tensorflow_datasets as tfds
#tfds.disable_progress_bar() #а надо ли дизейблить?
from tensorflow.keras import layers
import keras 
from keras import backend as K

from PIL import Image
from skimage.transform import resize
x = Image.open('six.png')
x = np.invert(x)
x = np.array(x)[ :, :, 1]
x_new = np.zeros((28,28))
x_new[2:26, 2:26] = x
x = x_new.astype('float32')
x /= 255
x = x.reshape(1,28,28,1)
print(x)
np.savetxt('image_six.txt',x.flatten(), fmt='%1.35f', newline=', ')

#импортим модель и смотрим на внутренности
export_path_sm = "./cnn.h5"
print(export_path_sm)
MyModel = tf.keras.models.load_model(export_path_sm)
MyModel.summary()

#чекаем что модель работает правильно
out = MyModel.predict(x)
print(out)

#функция дающая output conv_2d_1 в нужном формате
get_1st_layer_output = K.function([MyModel.layers[0].input],
                                  [MyModel.layers[0].output])
layer_output = get_1st_layer_output([x])[0]
#type(layer_output)
#чекаем размерности
conv_2d_1_orig_output=get_1st_layer_output([x])[0]
print("layer shape=",layer_output.shape)

layer_output=np.swapaxes(layer_output,2,3)
print("layer shape=",layer_output.shape)
layer_output=np.swapaxes(layer_output,1,2)
print("layer shape=",layer_output.shape)
layer_output=np.squeeze(layer_output)
print("layer shape=",layer_output.shape)
#print(layer_output)
#layer_output=swapaxes(layer_output,1,2)
#layer_output_reshaped=swapaxes(layer_output,1,3)
#print("layer dimesions=", layer_output_reshaped.ndim)
#print("layer shape=",layer_output_reshaped.shape)
#print(layer_output_reshaped)
#ресайз
#layer_output_reshaped=np.reshape(layer_output, (676,32))
#сохраняем в файл
#np.savetxt("conv2Doutput",layer_output_reshaped)
#tf.print(layer_output)

#функция дающая output conv_2d_2 в нужном формате
get_2nd_layer_output = K.function([MyModel.layers[1].input],
                                  [MyModel.layers[1].output])
layer_output2 = get_2nd_layer_output([conv_2d_1_orig_output])[0]
conv_2d_2_orig_output=get_2nd_layer_output([conv_2d_1_orig_output])[0]
#type(layer_output)
#чекаем размерности
print("layer2 shape=",layer_output2.shape)
layer_output2=np.swapaxes(layer_output2,2,3)
print("layer2 shape=",layer_output2.shape)
layer_output2=np.swapaxes(layer_output2,1,2)
print("layer2 shape=",layer_output2.shape)
layer_output2=np.squeeze(layer_output2)
print("layer2 shape=",layer_output2.shape)

#layer_output2=np.swapaxes(layer_output2,2,3)
#print("layer shape=",layer_output2.shape)
#layer_output2=np.swapaxes(layer_output2,1,2)
#print("layer shape=",layer_output2.shape)
#layer_output2=np.squeeze(layer_output2)
#print("layer shape=",layer_output2.shape)
#print(layer2_output)



#функция дающая output max_pool_2d_1 в нужном формате
get_3rd_layer_output = K.function([MyModel.layers[2].input],
                                  [MyModel.layers[2].output])
layer_output3 = get_3rd_layer_output([conv_2d_2_orig_output])[0]
max_poool_2d_1_orig_output=get_3rd_layer_output([conv_2d_2_orig_output])[0]
#type(layer_output)
#чекаем размерности
print("layer3 shape=",layer_output3.shape)
layer_output3=np.swapaxes(layer_output3,2,3)
print("layer3 shape=",layer_output3.shape)
layer_output3=np.swapaxes(layer_output3,1,2)
print("layer3 shape=",layer_output3.shape)
layer_output3=np.squeeze(layer_output3)
print("layer3 shape=",layer_output3.shape)

##слои без параметров
get_3dot5_layer_output = K.function([MyModel.layers[3].input],
                                  [MyModel.layers[3].output])
dropout_orig_output=get_3dot5_layer_output([max_poool_2d_1_orig_output])[0]
get_4_layer_output = K.function([MyModel.layers[4].input],
                                  [MyModel.layers[4].output])
flatten_orig_output=get_4_layer_output([dropout_orig_output])[0]



#функция дающая output dense_1 в нужном формате
get_5th_layer_output = K.function([MyModel.layers[5].input],
                                  [MyModel.layers[5].output])
layer_output5 = get_5th_layer_output([flatten_orig_output])[0]
dense_1_orig_output=get_5th_layer_output([flatten_orig_output])[0]
#type(layer_output)
#чекаем размерности
print("layer_output5 shape=",layer_output5.shape)
layer_output5=np.squeeze(layer_output5)
print("layer_output5 shape=",layer_output5.shape)


##слой без параметров
get_6_layer_output = K.function([MyModel.layers[6].input],
                                  [MyModel.layers[6].output])
dropout2_orig_output=get_6_layer_output([dense_1_orig_output])[0]

#функция дающая output dense_2 в нужном формате
get_7th_layer_output = K.function([MyModel.layers[7].input],
                                  [MyModel.layers[7].output])
layer_output7 = get_7th_layer_output([dropout2_orig_output])[0]
dense_2_orig_output=get_7th_layer_output([dropout2_orig_output])[0]
#type(layer_output)
#чекаем размерности
print("layer_output7 shape=",layer_output7.shape)
layer_output7=np.squeeze(layer_output7)
print("layer_output7 shape=",layer_output7.shape)

from numpy.core.fromnumeric import swapaxes
##кернелы conv_2d_1
LAYER=MyModel.layers[0]
KERNEL=LAYER.get_weights()[0]
#KERNEL_swapped=np.empty((3,1,3,32), dtype=float)
print("KERNEL dimensions = ", KERNEL.shape[0])
#print("KERNEL_swapped dimensions = ", KERNEL_swapped.shape)
#KERNEL_swapped=swapaxes(KERNEL,1,2)
#print(KERNEL_swapped)

#KERNEL_swapped2=np.empty((32,1,3,3), dtype=float)
#print("KERNEL dimensions = ", KERNEL.shape)
#print("KERNEL_swapped dimensions = ", KERNEL_swapped.shape)
#print("KERNEL_swapped2 dimensions = ", KERNEL_swapped2.shape)
#KERNEL_swapped2_conv_2d_1=swapaxes(KERNEL_swapped,0,3)
#KERNEL_swapped2_conv_2d_1=swapaxes(KERNEL_swapped,0,3)
#print(KERNEL_swapped2_conv_2d_1) #кернел с измененными измерениями 
#теперь придётся выпрямлять в вектор т.к. numpy не сохраняет n-dimensional массивы

##баесы conv_2d_1
LAYER=MyModel.layers[0]
BIASES_conv_2d_1=LAYER.get_weights()[1]
#print('BIASES dimensions = ' , BIASES_conv_2d_1.shape)
#print(BIASES_conv_2d_1)


##кернелы conv_2d_2
LAYER1=MyModel.layers[1]
KERNEL1=LAYER1.get_weights()[0]
#KERNEL1_swapped=np.empty((3,64,3,32), dtype=float)
print("KERNEL1 dimensions = ",KERNEL1.shape)
#print("KERNEL1_swapped dimensions = ", KERNEL1_swapped.shape)
#KERNEL1_swapped=swapaxes(KERNEL1,1,2)
#print("KERNEL1_swapped dimensions = ",KERNEL1_swapped.shape)
#sprint(KERNEL1_swapped)
#KERNEL1_swapped=swapaxes(KERNEL1_swapped,0,3)
#print("KERNEL1_swapped dimensions = ", KERNEL1_swapped.shape)
#KERNEL1_swapped=swapaxes(KERNEL1_swapped,1,0)
#print(KERNEL1_swapped)

##баесы conv_2d_2
BIASES_conv_2d_2=LAYER1.get_weights()[1]
print("BIASES_conv_2d_2 dimensions = ", BIASES_conv_2d_2.shape)

##коэффициенты dense_1
COEFF_dense_1=MyModel.layers[5].get_weights()[0]
print("COEFF_dense_1 dimensioms = ",COEFF_dense_1.shape)
COEFF_dense_1=swapaxes(COEFF_dense_1,0,1)
print("COEFF_dense_1 dimensioms = ",COEFF_dense_1.shape)

##баесы dense_1
BIASES_dense_1=MyModel.layers[5].get_weights()[1]
print("BIASES_dense_1 dimensions = ",BIASES_dense_1.shape)

##коэффициенты dense_2
COEFF_dense_2=MyModel.layers[7].get_weights()[0]
print("COEFF_dense_2 dimensioms = ",COEFF_dense_2.shape)
COEFF_dense_2=swapaxes(COEFF_dense_2,0,1)
print("COEFF_dense_2 dimensioms = ",COEFF_dense_2.shape)

##баесы dense_1
BIASES_dense_2=MyModel.layers[7].get_weights()[1]
print("BIASES_dense_2 dimensions = ",BIASES_dense_2.shape)


#экспорт кернела conv_2d_1
#print(KERNEL_swapped2_conv_2d_1)
KERNEL_conv_2d_1=KERNEL
#print(KERNEL_swapped2_conv_2d_1)
#print(KERNEL_swapped2)
np.savetxt('conv_2d_1_kernels.txt',KERNEL_conv_2d_1.flatten(), fmt='%1.35f', newline=' ')

#экспорт баесов conv_2d_1
#print(BIASES_conv_2d_1)
np.savetxt('conv_2d_1_biases.txt', BIASES_conv_2d_1, fmt='%1.35f', newline=' ')

#экспорт вывода conv_2d_1
conv_2d_1_output=layer_output
np.savetxt('conv_2d_1_output_flat.txt',conv_2d_1_output.flatten(),  fmt='%1.35f' , newline='\n ')

#экспорт кернела conv_2d_2
np.savetxt('conv_2d_2_kernels.txt',KERNEL1.flatten(), fmt='%1.35f', newline=' ')

#экспорт баесов conv_2d_2
print(BIASES_conv_2d_2)
np.savetxt('conv_2d_2_biases.txt', BIASES_conv_2d_2, fmt='%1.35f', newline=' ')

#экспорт вывода conv_2d_2
conv_2d_2_output=layer_output2
np.savetxt('conv_2d_2_output_flat.txt',conv_2d_2_output.flatten(),  fmt='%1.35f', newline='\n ')

#экспорт вывода max_pool_2d_1
max_pool_2d_1_output=layer_output3
np.savetxt('max_pool_2d_1_output_flat.txt',max_pool_2d_1_output.flatten(),  fmt='%1.35f', delimiter=',' , newline=' ')

np.savetxt('dropout_output_flat.txt',dropout_orig_output.flatten(),  fmt='%1.35f', delimiter=',' , newline=' ')


#экспорт коэффициентов dense_1
np.savetxt('dense_1_coeff.txt', COEFF_dense_1, fmt='%1.35f', newline=' ')

#экспорт баесов dense_1
np.savetxt('dense_1_biases.txt', BIASES_dense_1, fmt='%1.35f', newline=' ')

#экспорт коэффициентов dense_2
np.savetxt('dense_2_coeff.txt', COEFF_dense_2, fmt='%1.35f', newline=' ')

#экспорт баесов dense_2
np.savetxt('dense_2_biases.txt', BIASES_dense_2, fmt='%1.35f', newline=' ')

# # Запись массива в читабильном виде для визуального сравнения
# with open('conv_2d_1_output.txt', 'w') as outfile:
#     # I'm writing a header here just for the sake of readability
#     # Any line starting with "#" will be ignored by numpy.loadtxt
#     outfile.write('# Array shape: {0}\n'.format(conv_2d_1_orig_output.shape))
    
#     # Iterating through a ndimensional array produces slices along
#     # the last axis. This is equivalent to data[i,:,:] in this case
#     for data_slice in conv_2d_1_output:

#         # The formatting string indicates that I'm writing out
#         # the values in left-justified columns 7 characters in width
#         # with 2 decimal places.  
#         np.savetxt(outfile, data_slice, fmt='%10.25f')

#         # Writing out a break to indicate different slices...
#         outfile.write('# New slice\n')

# # Запись массива в читабильном виде для визуального сравнения
# with open('conv_2d_2_output.txt', 'w') as outfile:
#     # I'm writing a header here just for the sake of readability
#     # Any line starting with "#" will be ignored by numpy.loadtxt
#     outfile.write('# Array shape: {0}\n'.format(conv_2d_2_orig_output.shape))
    
#     # Iterating through a ndimensional array produces slices along
#     # the last axis. This is equivalent to data[i,:,:] in this case
#     for data_slice in conv_2d_2_output:

#         # The formatting string indicates that I'm writing out
#         # the values in left-justified columns 7 characters in width
#         # with 2 decimal places.  
#         np.savetxt(outfile, data_slice, fmt='%10.25f')

#         # Writing out a break to indicate different slices...
#         outfile.write('# New slice\n')

# # Запись массива в читабильном виде для визуального сравнения
# with open('max_pool_2d_1_output.txt', 'w') as outfile:
#     # I'm writing a header here just for the sake of readability
#     # Any line starting with "#" will be ignored by numpy.loadtxt
#     outfile.write('# Array shape: {0}\n'.format(max_pool_2d_1_output.shape))
    
#     # Iterating through a ndimensional array produces slices along
#     # the last axis. This is equivalent to data[i,:,:] in this case
#     for data_slice in max_pool_2d_1_output:

#         # The formatting string indicates that I'm writing out
#         # the values in left-justified columns 7 characters in width
#         # with 2 decimal places.  
#         np.savetxt(outfile, data_slice, fmt='%10.25f')

#         # Writing out a break to indicate different slices...
#         outfile.write('# New slice\n')

#выход dense_1
np.savetxt('dense_1_output.txt', layer_output5,  fmt='%1.35f', newline=' ')

#выход dense_2
np.savetxt('dense_2_output.txt', layer_output7,  fmt='%1.35f', newline=' ')

import keract
from keract import get_activations
activations = get_activations(MyModel, x, auto_compile=True)
[print(k, '->', v.shape, '- Numpy array') for (k, v) in activations.items()]


#графическое изображения выводов слоёв
keract.display_activations(activations, cmap=None, save=False, directory='.', 
                           data_format='channels_last', fig_size=(10, 10), reshape_1d_layers=False)
