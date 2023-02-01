DEV_CODE_SOURCES = \
dev_code/entry/src/entry.c \
dev_code/task/driver_task/src/driver_task.c \
dev_code/task/app_task/src/app_task.c \
dev_code/component/led/src/led.c \
dev_code/component/trace/src/trace.c


DEV_CODE_INCLUDES = \
-Idev_code/entry/inc \
-Idev_code/task/driver_task/inc \
-Idev_code/task/app_task/inc \
-Idev_code/component/led/inc \
-Idev_code/component/trace/inc \
-Idev_code/component/common/inc