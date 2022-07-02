# DarkMoon

# 使用指南
1.在根目录双击.bat生成vs工程文件

2.由于glfw库当前版本会有lnk2019 error所以有以下临时解决办法：
    a.需要手动拷贝glfw/premake5.lua到其他文件夹
    b.然后把glfw库的分支切换到3.3-stable分支
    c.然后把拷贝出去的premake5.lua复制回来
    d.重复上面1的动作
