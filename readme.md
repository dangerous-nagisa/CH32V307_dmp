# README

本项目基于`CH32V307VCT6`,移植了`mpu6050`的`dmp`库。使用软件`iic`，更改引脚仅需更改I2C写SCL电平，写SDA，读取SDA。`driver`文件夹下的`mpu6050`文件夹为`mpu6050`所需全部文件。

本项目使用任务调度器，使用定时器三进行系统计时`uwtick`。更改应用层只需要更改`mpu6050.c`下的`mpu6050_task()`函数。

```c
 void mpu6050_task(void)
{
    int ret = 0;
    // ret = MPU6050_DMP_init();
    dmp_getwalk();
    if(ret == 0){
        for (int i = 0; i < 10; i++) {  // 尝试读取10次
            printf("\n--- 读取尝试 #%d ---\n", i+1);
            
            ret = MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
            
            if (ret == 0) {
                printf("成功! 姿态角: Pitch=%d° Roll=%d° Yaw=%d°\r\n", 
                    (int)(pitch * 100), (int)(roll * 100), (int)(yaw * 100));
            } else {
                printf("读取失败，错误码: %d\r\n", ret);
            }
        }
    }  
}
```

参考链接：https://blog.csdn.net/m0_56841583/article/details/147332035?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-1-147332035-blog-88352427.235^v43^pc_blog_bottom_relevance_base4&spm=1001.2101.3001.4242.2&utm_relevant_index=3






