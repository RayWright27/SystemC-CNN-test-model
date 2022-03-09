# SystemC-CNN-test-model
This is an **unfinished** test model of CNN, based on cnn.h5 Keras pretrained model https://github.com/EN10/KerasMNIST/blob/4ef71d6332e12376df93c96b39cf01ffbe48d02a/cnn.h5 .

This model serves as a testing ground for making Keras models in SystemC and contains the following layers:

![Alt Text](https://github.com/RayWright27/SystemC-CNN-test-model/blob/clocked/System-C%20model.png)

tb_driver.h contains image of number 6 in form of grayscale values matix and arbitrary kernel.
All interfaces are 1d vectors, so on inputs/output ports of some modules 1d vectors transform into 2d matrices and vice versa.

Convolution is realised with element-wise, for-loop multiplication of kernel and image instead of multiplication of matricies using Toepliz matricies. This is the issue to be solved.


