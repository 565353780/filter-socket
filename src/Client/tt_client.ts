<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>ClientDialog</name>
    <message>
        <location filename="clientdialog.ui" line="14"/>
        <source>Dialog</source>
        <translation>客户端</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="31"/>
        <source>Solve Mode:</source>
        <translatorcomment>选择深度学习模式，以启用不同的学习策略和网络模型</translatorcomment>
        <translation>深度学习模式：</translation>
    </message>
    <message>
        <source>LapNet</source>
        <translatorcomment>LapNet模式</translatorcomment>
        <translation type="vanished">LapNet</translation>
    </message>
    <message>
        <source>DarkNet</source>
        <translatorcomment>DarkNet模式-</translatorcomment>
        <translation type="vanished">DarkNet</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="41"/>
        <source>Conda Env Name:</source>
        <translatorcomment>输入安装有torch及tensorflow的Anaconda环境名称来启用训练功能</translatorcomment>
        <translation>Anaconda环境名：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="51"/>
        <source>Port</source>
        <translatorcomment>输入端口号来与服务器进行匹配，暂时默认为13141</translatorcomment>
        <translation>端口</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="58"/>
        <source>13141</source>
        <translation></translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="122"/>
        <location filename="ClientDialog.cpp" line="78"/>
        <source>Mask</source>
        <translatorcomment>标定分栏</translatorcomment>
        <translation>标定</translation>
    </message>
    <message>
        <source>Open Mask Config</source>
        <translatorcomment>点击此按钮来在新窗口中打开标定工具</translatorcomment>
        <translation type="vanished">打开标定工具</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="150"/>
        <source>Rect</source>
        <translatorcomment>矩形标记分栏</translatorcomment>
        <translation>矩形</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="170"/>
        <source>Poly</source>
        <translatorcomment>多边形标记分栏</translatorcomment>
        <translation>多边形</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="191"/>
        <source>Data Manage</source>
        <translatorcomment>数据管理分栏</translatorcomment>
        <translation>数据管理</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="203"/>
        <source>Folder Type</source>
        <translatorcomment>通过下拉框选择要发送的文件类型，注意与深度学习模式进行匹配，不同学习模式下传送的文件在训练时不进行共享</translatorcomment>
        <translation>文件类型：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="230"/>
        <source>Dataset</source>
        <translatorcomment>选择待发送文件为数据集文件夹</translatorcomment>
        <translation>数据集</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="235"/>
        <source>OBJ</source>
        <translatorcomment>选择待发送文件为OBJ模型文件</translatorcomment>
        <translation>OBJ模型</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="249"/>
        <source>Send Folder</source>
        <translatorcomment>单击来进行所选文件夹的打包发送</translatorcomment>
        <translation>发送文件夹</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="262"/>
        <source>Get Files Message</source>
        <translatorcomment>单击来获取服务器上所保存和生成的所有文件的路径信息</translatorcomment>
        <translation>获取服务器文件信息</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="285"/>
        <source>Clear All</source>
        <translatorcomment>单击来删除服务器上生成的所有文件</translatorcomment>
        <translation>清除所有服务器文件</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="302"/>
        <location filename="clientdialog.ui" line="340"/>
        <source>Train</source>
        <translatorcomment>训练分栏</translatorcomment>
        <translation>训练</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="314"/>
        <source>Get Latest Result</source>
        <translatorcomment>单击来获取最新训练好的网络模型，目前暂时仅支持LapNet</translatorcomment>
        <translation>获取最新模型</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="327"/>
        <source>Stop Train</source>
        <translatorcomment>单击来终止训练过程，目前仅支持LapNet</translatorcomment>
        <translation>终止训练</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="353"/>
        <source>Start Enhancement</source>
        <translatorcomment>单击来开始数据增强功能，目前仅支持DarkNet</translatorcomment>
        <translation>开始数据增强</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="366"/>
        <source>Enhancement Factor:</source>
        <translatorcomment>输入正整数来控制数据增强的数量，目标数据量=已有数据量*数据增强系数</translatorcomment>
        <translation>数据增强系数：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="389"/>
        <source>Max OBJ Num / Img :</source>
        <translatorcomment>输入正整数来控制每张图片中最多可以生成的OBJ数量</translatorcomment>
        <translation>最大OBJ数量/图：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="412"/>
        <source>Start DataChange</source>
        <translatorcomment>单击来将所有的原始数据转换为完整COCO数据集，暂时仅支持DarkNet</translatorcomment>
        <translation>开始数据转换</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="425"/>
        <source>Create Data Num   :</source>
        <translatorcomment>输入正整数来控制从OBJ文件自动生成数据集的大小</translatorcomment>
        <translation>数据生成数量：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="448"/>
        <source>Start Create Data</source>
        <translatorcomment>单击来开始自动生成原始数据集</translatorcomment>
        <translation>开始数据生成</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="461"/>
        <source>Label Num :</source>
        <translatorcomment>标签数量：</translatorcomment>
        <translation>标签数量：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="494"/>
        <source>Train / Total(%) :</source>
        <translatorcomment>训练/全部：</translatorcomment>
        <translation>训练/全部：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="507"/>
        <source>Valid</source>
        <translatorcomment>评估</translatorcomment>
        <translation>评估</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="520"/>
        <source>Set Model Result</source>
        <translatorcomment>设置训练模型（ToDo）</translatorcomment>
        <translation>设置训练模型（ToDo）</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="549"/>
        <source>Test</source>
        <translatorcomment>测试分栏</translatorcomment>
        <translation>测试</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="561"/>
        <source>Output Image</source>
        <translatorcomment>此框中将显示网络预测结果</translatorcomment>
        <translation>输出图片</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="577"/>
        <source>Darknet Process</source>
        <translatorcomment>单击来进行DarkNet的预测，仅在Linux系统上可用</translatorcomment>
        <translation>DarkNet预测(GPU)</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="590"/>
        <source>LapNet Process</source>
        <translatorcomment>单击来进行LapNet的预测，在Windows系统和Linux系统上均可用</translatorcomment>
        <translation>LapNet预测(GPU)</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="651"/>
        <source>Poly Yolo Process</source>
        <translatorcomment>Poly-Yolo预测(GPU)</translatorcomment>
        <translation>Poly-Yolo预测(GPU)</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="664"/>
        <source>Darknet Test</source>
        <translatorcomment>单击来启动Darknet测试(GPU)</translatorcomment>
        <translation>Darknet测试(GPU)</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="690"/>
        <source>Load Net</source>
        <translatorcomment>选择是否加载网络</translatorcomment>
        <translation>加载网络</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="703"/>
        <source>Thresh :</source>
        <translatorcomment>阈值：</translatorcomment>
        <translation>阈值：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="726"/>
        <location filename="clientdialog.ui" line="762"/>
        <source>Image Dir :</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="775"/>
        <location filename="clientdialog.ui" line="788"/>
        <source>Select</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="843"/>
        <source>Show Mask</source>
        <translatorcomment>激活来显示遮罩</translatorcomment>
        <translation>显示遮罩</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="856"/>
        <source>Save Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="862"/>
        <source>Camera Pose Initialization</source>
        <translatorcomment>相机位姿初始化分栏</translatorcomment>
        <translation>相机位姿初始化</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="885"/>
        <source>Choose Point Pair</source>
        <translation></translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="898"/>
        <source>Get Camera Frame</source>
        <translatorcomment>单击来获取相机画面</translatorcomment>
        <translation>获取相机画面</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="911"/>
        <source>Init Camera</source>
        <translatorcomment>单击来初始化相机</translatorcomment>
        <translation>初始化相机</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="924"/>
        <source>detect density:</source>
        <translatorcomment>目标检测密度</translatorcomment>
        <translation>目标检测密度：</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="940"/>
        <source>Data Manager</source>
        <translatorcomment>数据库管理分栏</translatorcomment>
        <translation>数据库管理</translation>
    </message>
    <message>
        <source>Yolov3 Process</source>
        <translatorcomment>单击来进行DarkNet的预测，仅在Windows系统上可用</translatorcomment>
        <translation type="vanished">DarkNet预测(OpenVINO)</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="603"/>
        <source>OpenImage</source>
        <translatorcomment>单击来选择要进行测试的图片</translatorcomment>
        <translation>打开图片</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="622"/>
        <source>Input Image</source>
        <translatorcomment>此框中将显示打开的图片</translatorcomment>
        <translation>输入图片</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="638"/>
        <source>Init Model</source>
        <translatorcomment>单击来进行网络模型初始化，更新至最新训练的网络</translatorcomment>
        <translation>初始化网络</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="819"/>
        <source>Stop Camera Cap</source>
        <translatorcomment>单击来停止摄像头监控</translatorcomment>
        <translation>停止摄像头监控</translation>
    </message>
    <message>
        <source>Stop Detect</source>
        <translatorcomment>单击来停止可能正在进行的连续预测功能</translatorcomment>
        <translation type="vanished">停止预测</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="794"/>
        <source>Camera Cap</source>
        <translatorcomment>监控分栏</translatorcomment>
        <translation>监控</translation>
    </message>
    <message>
        <location filename="clientdialog.ui" line="806"/>
        <source>Start Camera Cap</source>
        <translatorcomment>单击来开启摄像头监控</translatorcomment>
        <translation>开启摄像头监控</translation>
    </message>
    <message>
        <source>People Detect Img</source>
        <translatorcomment>显示目标检测图片</translatorcomment>
        <translation type="vanished">目标检测图片</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="9"/>
        <source>ClientDialog</source>
        <translatorcomment>客户端窗口</translatorcomment>
        <translation>客户端</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="259"/>
        <source>Socket Connected </source>
        <translation>通信已连接</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="259"/>
        <location filename="ClientDialog.cpp" line="264"/>
        <source>:</source>
        <translation>:</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="264"/>
        <source>Socket Disconnected </source>
        <translation>通信已断开</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="269"/>
        <source>Socket Error </source>
        <translation>通信错误</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="1156"/>
        <location filename="ClientDialog.cpp" line="1159"/>
        <source>Cost time: </source>
        <translation>耗时：</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="1156"/>
        <location filename="ClientDialog.cpp" line="1159"/>
        <source>ms</source>
        <translation>毫秒</translation>
    </message>
    <message>
        <location filename="ClientDialog.cpp" line="1249"/>
        <location filename="ClientDialog.cpp" line="1318"/>
        <location filename="ClientDialog.cpp" line="1453"/>
        <location filename="ClientDialog.cpp" line="1523"/>
        <source>bbox size : </source>
        <translation>矩形框数量：</translation>
    </message>
</context>
<context>
    <name>DataManager</name>
    <message>
        <location filename="DataManager.cpp" line="155"/>
        <location filename="DataManager.cpp" line="176"/>
        <location filename="DataManager.cpp" line="188"/>
        <location filename="DataManager.cpp" line="200"/>
        <location filename="DataManager.cpp" line="215"/>
        <location filename="DataManager.cpp" line="228"/>
        <location filename="DataManager.cpp" line="241"/>
        <source>ImageID</source>
        <translation type="unfinished">图片ID</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="156"/>
        <source>ImagePath</source>
        <translation type="unfinished">图片路径</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="157"/>
        <source>ImageWidth</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="158"/>
        <source>ImageHeight</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="159"/>
        <source>ImportTime</source>
        <translation type="unfinished">载入时间</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="160"/>
        <source>isYolov3RegionMasked</source>
        <translation type="unfinished">Yolov3区域已标注</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="161"/>
        <source>Yolov3RegionMaskID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="162"/>
        <source>isYolov3ComponentMasked</source>
        <translation type="unfinished">Yolov3部件已标注</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="163"/>
        <source>Yolov3ComponentMaskID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="164"/>
        <source>isResNetClassificationMasked</source>
        <translation type="unfinished">ResNet分类已标注</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="165"/>
        <source>ResNetClassificationMaskID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="166"/>
        <source>isYolov3RegionPredicted</source>
        <translation type="unfinished">Yolov3区域已预测</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="167"/>
        <source>isYolov3ComponentPredicted</source>
        <translation type="unfinished">Yolov3部件已预测</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="168"/>
        <source>isResNetClassificationPredicted</source>
        <translation type="unfinished">ResNet分类已预测</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="177"/>
        <location filename="DataManager.cpp" line="189"/>
        <location filename="DataManager.cpp" line="201"/>
        <location filename="DataManager.cpp" line="216"/>
        <location filename="DataManager.cpp" line="229"/>
        <location filename="DataManager.cpp" line="242"/>
        <source>ID</source>
        <translation type="unfinished">序号</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="178"/>
        <location filename="DataManager.cpp" line="190"/>
        <location filename="DataManager.cpp" line="202"/>
        <location filename="DataManager.cpp" line="217"/>
        <location filename="DataManager.cpp" line="230"/>
        <location filename="DataManager.cpp" line="243"/>
        <source>Time</source>
        <translation type="unfinished">时间</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="179"/>
        <location filename="DataManager.cpp" line="191"/>
        <location filename="DataManager.cpp" line="203"/>
        <location filename="DataManager.cpp" line="218"/>
        <location filename="DataManager.cpp" line="231"/>
        <location filename="DataManager.cpp" line="244"/>
        <source>label</source>
        <translation type="unfinished">标签</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="180"/>
        <location filename="DataManager.cpp" line="192"/>
        <location filename="DataManager.cpp" line="204"/>
        <location filename="DataManager.cpp" line="219"/>
        <location filename="DataManager.cpp" line="232"/>
        <location filename="DataManager.cpp" line="245"/>
        <source>x1</source>
        <translation type="unfinished">x1</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="181"/>
        <location filename="DataManager.cpp" line="193"/>
        <location filename="DataManager.cpp" line="205"/>
        <location filename="DataManager.cpp" line="220"/>
        <location filename="DataManager.cpp" line="233"/>
        <location filename="DataManager.cpp" line="246"/>
        <source>y1</source>
        <translation type="unfinished">y1</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="182"/>
        <location filename="DataManager.cpp" line="194"/>
        <location filename="DataManager.cpp" line="206"/>
        <location filename="DataManager.cpp" line="221"/>
        <location filename="DataManager.cpp" line="234"/>
        <location filename="DataManager.cpp" line="247"/>
        <source>x2</source>
        <translation type="unfinished">x2</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="183"/>
        <location filename="DataManager.cpp" line="195"/>
        <location filename="DataManager.cpp" line="207"/>
        <location filename="DataManager.cpp" line="222"/>
        <location filename="DataManager.cpp" line="235"/>
        <location filename="DataManager.cpp" line="248"/>
        <source>y2</source>
        <translation type="unfinished">y2</translation>
    </message>
    <message>
        <location filename="DataManager.cpp" line="223"/>
        <location filename="DataManager.cpp" line="236"/>
        <location filename="DataManager.cpp" line="249"/>
        <source>score</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DataManagerWidget</name>
    <message>
        <location filename="datamanagerwidget.ui" line="14"/>
        <source>Form</source>
        <translatorcomment>数据库管理界面</translatorcomment>
        <translation>数据库管理界面</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="26"/>
        <source>Open Dir</source>
        <translatorcomment>打开目录</translatorcomment>
        <translation>打开目录</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="43"/>
        <source>SourceImage</source>
        <translatorcomment>原始图片</translatorcomment>
        <translation>原始图片</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="58"/>
        <source>Mask</source>
        <translatorcomment>标注</translatorcomment>
        <translation>标注</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="74"/>
        <location filename="datamanagerwidget.ui" line="137"/>
        <location filename="datamanagerwidget.ui" line="387"/>
        <source>Yolov3Region</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="89"/>
        <location filename="datamanagerwidget.ui" line="152"/>
        <location filename="datamanagerwidget.ui" line="392"/>
        <source>Yolov3Component</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="104"/>
        <location filename="datamanagerwidget.ui" line="167"/>
        <location filename="datamanagerwidget.ui" line="397"/>
        <source>ResNetClassification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="121"/>
        <source>Predict</source>
        <translatorcomment>预测</translatorcomment>
        <translation>预测</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="206"/>
        <source>Create Work Station</source>
        <translation type="unfinished">创建工作空间</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="229"/>
        <source>Workers Num :</source>
        <translation type="unfinished">标注人数量：</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="252"/>
        <source>Target Dataset Size :</source>
        <translation type="unfinished">目标数据集大小：</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="265"/>
        <source>Recycle Mask</source>
        <translation type="unfinished">回收标注数据</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="288"/>
        <source>Target Dataset Path :</source>
        <translation type="unfinished">目标数据集路径：</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="311"/>
        <source>Recycle Valid</source>
        <translation type="unfinished">回收有效数据集</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="324"/>
        <source>Create Valid Dataset</source>
        <translation type="unfinished">创建有效数据集</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="337"/>
        <source>Source Dataset Path :</source>
        <translation type="unfinished">源数据集路径：</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="350"/>
        <source>Init Param</source>
        <translation type="unfinished">初始化参数</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="373"/>
        <source>Create Train Dataset</source>
        <translation type="unfinished">创建训练数据集</translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="411"/>
        <source>Mask Mode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="424"/>
        <source>Row Index :</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="447"/>
        <source>Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="460"/>
        <source>insert :</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="473"/>
        <source>Select</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="486"/>
        <source>connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datamanagerwidget.ui" line="499"/>
        <source>load num :</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ImageID</source>
        <translatorcomment>图片ID</translatorcomment>
        <translation type="vanished">图片ID</translation>
    </message>
    <message>
        <source>ImagePath</source>
        <translatorcomment>图片路径</translatorcomment>
        <translation type="vanished">图片路径</translation>
    </message>
    <message>
        <source>ImportTime</source>
        <translatorcomment>载入时间</translatorcomment>
        <translation type="vanished">载入时间</translation>
    </message>
    <message>
        <source>isYolov3RegionMasked</source>
        <translatorcomment>Yolov3区域已标注</translatorcomment>
        <translation type="vanished">Yolov3区域已标注</translation>
    </message>
    <message>
        <source>isYolov3ComponentMasked</source>
        <translatorcomment>Yolov3部件已标注</translatorcomment>
        <translation type="vanished">Yolov3部件已标注</translation>
    </message>
    <message>
        <source>isResNetClassificationMasked</source>
        <translatorcomment>ResNet分类已标注</translatorcomment>
        <translation type="vanished">ResNet分类已标注</translation>
    </message>
    <message>
        <source>isYolov3RegionPredicted</source>
        <translatorcomment>Yolov3区域已预测</translatorcomment>
        <translation type="vanished">Yolov3区域已预测</translation>
    </message>
    <message>
        <source>isYolov3ComponentPredicted</source>
        <translatorcomment>Yolov3部件已预测</translatorcomment>
        <translation type="vanished">Yolov3部件已预测</translation>
    </message>
    <message>
        <source>isResNetClassificationPredicted</source>
        <translatorcomment>ResNet分类已预测</translatorcomment>
        <translation type="vanished">ResNet分类已预测</translation>
    </message>
    <message>
        <source>ID</source>
        <translatorcomment>序号</translatorcomment>
        <translation type="vanished">序号</translation>
    </message>
    <message>
        <source>Time</source>
        <translatorcomment>时间</translatorcomment>
        <translation type="vanished">时间</translation>
    </message>
    <message>
        <source>label</source>
        <translatorcomment>标签</translatorcomment>
        <translation type="vanished">标签</translation>
    </message>
    <message>
        <source>x1</source>
        <translatorcomment>x1</translatorcomment>
        <translation type="vanished">x1</translation>
    </message>
    <message>
        <source>x2</source>
        <translatorcomment>x2</translatorcomment>
        <translation type="vanished">x2</translation>
    </message>
    <message>
        <source>y1</source>
        <translatorcomment>y1</translatorcomment>
        <translation type="vanished">y1</translation>
    </message>
    <message>
        <source>y2</source>
        <translatorcomment>y2</translatorcomment>
        <translation type="vanished">y2</translation>
    </message>
</context>
<context>
    <name>MaskBBoxInfoWidget</name>
    <message>
        <location filename="maskbboxinfowidget.ui" line="14"/>
        <source>Form</source>
        <translatorcomment>多边形标注界面</translatorcomment>
        <translation>多边形标注界面</translation>
    </message>
</context>
<context>
    <name>MaskConfig::BindingWidget</name>
    <message>
        <location filename="../MaskConfig/WIdgets/BindingWidget.cpp" line="18"/>
        <source>Next</source>
        <translatorcomment>单击切换下一个矩形框</translatorcomment>
        <translation>下一个</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/BindingWidget.cpp" line="47"/>
        <source>None</source>
        <translatorcomment>无</translatorcomment>
        <translation>无</translation>
    </message>
</context>
<context>
    <name>MaskConfig::ConfigWidget</name>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="39"/>
        <source>OpenDir</source>
        <translatorcomment>单击来打开目录</translatorcomment>
        <translation>打开目录</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="40"/>
        <source>Prev</source>
        <translatorcomment>单击来切换上一张图</translatorcomment>
        <translation>上一张图</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="43"/>
        <source>Next</source>
        <translatorcomment>单击来切换下一张图</translatorcomment>
        <translation>下一张图</translation>
    </message>
    <message>
        <source>LoadPrevResult</source>
        <translatorcomment>单击来加载之前的结果</translatorcomment>
        <translation type="vanished">加载之前的结果</translation>
    </message>
    <message>
        <source>Interpolation</source>
        <translatorcomment>单击来进行插值</translatorcomment>
        <translation type="vanished">插值</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="78"/>
        <source>Press0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="80"/>
        <source>Press1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="82"/>
        <source>Press2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="84"/>
        <source>Press3</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="86"/>
        <source>Press4</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="88"/>
        <source>Press5</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="90"/>
        <source>Press6</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="92"/>
        <source>Press7</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="94"/>
        <source>Press8</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="96"/>
        <source>Press9</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="98"/>
        <source>PressBS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="411"/>
        <source>Load Image...</source>
        <translatorcomment>加载图片...</translatorcomment>
        <translation>加载图片...</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="424"/>
        <source>Load Config...</source>
        <translatorcomment>加载配置文件...</translatorcomment>
        <translation>加载配置文件...</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="434"/>
        <source>Save Config...</source>
        <translatorcomment>保存配置文件...</translatorcomment>
        <translation>保存配置文件...</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="461"/>
        <source>Open Dir...</source>
        <translatorcomment>打开目录...</translatorcomment>
        <translation>打开目录...</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="518"/>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="666"/>
        <source>It&apos;s the last image!</source>
        <translatorcomment>这是最后一张图片!</translatorcomment>
        <translation>这是最后一张图片!</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="540"/>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="670"/>
        <source>It&apos;s the first image!</source>
        <translatorcomment>这是第一张图片!</translatorcomment>
        <translation>这是第一张图片!</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/ConfigWidget.cpp" line="564"/>
        <source>Get prev id</source>
        <translatorcomment>获取前一个ID</translatorcomment>
        <translation>获取前一个ID</translation>
    </message>
</context>
<context>
    <name>MaskConfig::Dict</name>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="40"/>
        <source>clear</source>
        <translatorcomment>清除</translatorcomment>
        <translation>清除</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="41"/>
        <source>PopBack</source>
        <translatorcomment>删除</translatorcomment>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="42"/>
        <source>SetClosed</source>
        <translatorcomment>设置为关闭</translatorcomment>
        <translation>设置为关闭</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="43"/>
        <source>PopPoint</source>
        <translatorcomment>删除点</translatorcomment>
        <translation>删除点</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="44"/>
        <source>PopPoly</source>
        <translatorcomment>删除框</translatorcomment>
        <translation>删除框</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="45"/>
        <source>Add</source>
        <translatorcomment>添加</translatorcomment>
        <translation>添加</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="46"/>
        <source>Prev</source>
        <translatorcomment>上一个BBox</translatorcomment>
        <translation>上一个BBox</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="47"/>
        <source>Next</source>
        <translatorcomment>下一个BBox</translatorcomment>
        <translation>下一个BBox</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="48"/>
        <source>ShowMode</source>
        <translatorcomment>显示模式</translatorcomment>
        <translation>显示模式</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="49"/>
        <source>Remove</source>
        <translatorcomment>删除当前BBox</translatorcomment>
        <translation>删除当前BBox</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="50"/>
        <source>SetNextId</source>
        <translatorcomment>设置下一个ID</translatorcomment>
        <translation>设置下一个ID</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="52"/>
        <source>Num</source>
        <translatorcomment>数量</translatorcomment>
        <translation>数量</translation>
    </message>
    <message>
        <location filename="../MaskConfig/Config/Dict.cpp" line="53"/>
        <source>None</source>
        <translatorcomment>无</translatorcomment>
        <translation>无</translation>
    </message>
</context>
<context>
    <name>MaskConfig::SetterWidget</name>
    <message>
        <location filename="../MaskConfig/WIdgets/SetterWidget.cpp" line="67"/>
        <source>Next</source>
        <translation>下一张图</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/SetterWidget.cpp" line="68"/>
        <source>Prev</source>
        <translation>上一张图</translation>
    </message>
</context>
<context>
    <name>MaskConfig::TrackIdWidget</name>
    <message>
        <location filename="../MaskConfig/WIdgets/TrackIdWidget.cpp" line="11"/>
        <source>NextId</source>
        <translatorcomment>下一个ID</translatorcomment>
        <translation>下一个ID</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/TrackIdWidget.cpp" line="12"/>
        <source>Id</source>
        <translatorcomment>ID</translatorcomment>
        <translation>ID</translation>
    </message>
</context>
<context>
    <name>MaskDatasetControlWidget</name>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="14"/>
        <source>Form</source>
        <translatorcomment>多边形标注界面</translatorcomment>
        <translation>多边形标注界面</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="36"/>
        <source>Target Dataset Path :</source>
        <translatorcomment>目标数据集路径：</translatorcomment>
        <translation>目标数据集路径：</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="49"/>
        <location filename="maskdatasetcontrolwidget.ui" line="134"/>
        <source>Browse</source>
        <translatorcomment>单击来浏览</translatorcomment>
        <translation>浏览</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="82"/>
        <source>Recycle Mask</source>
        <translatorcomment>回收标注数据</translatorcomment>
        <translation>回收标注数据</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="95"/>
        <source>Workers Num :</source>
        <translatorcomment>标注人数量：</translatorcomment>
        <translation>标注人数量：</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="108"/>
        <source>Create Work Station</source>
        <translatorcomment>单击来创建工作空间</translatorcomment>
        <translation>创建工作空间</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="121"/>
        <source>Source Dataset Path :</source>
        <translatorcomment>源数据集路径：</translatorcomment>
        <translation>源数据集路径：</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="147"/>
        <source>Target Dataset Size :</source>
        <translatorcomment>目标数据集大小：</translatorcomment>
        <translation>目标数据集大小：</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="180"/>
        <source>Create Train Dataset</source>
        <translatorcomment>单击来创建训练数据集</translatorcomment>
        <translation>创建训练数据集</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="193"/>
        <source>Init Param</source>
        <translatorcomment>单击来初始化参数</translatorcomment>
        <translation>初始化参数</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="206"/>
        <source>Create Valid Dataset</source>
        <translatorcomment>单击来创建有效数据集</translatorcomment>
        <translation>创建有效数据集</translation>
    </message>
    <message>
        <location filename="maskdatasetcontrolwidget.ui" line="219"/>
        <source>Recycle Valid</source>
        <translatorcomment>单击来回收有效数据集</translatorcomment>
        <translation>回收有效数据集</translation>
    </message>
</context>
<context>
    <name>MaskPolyWidget</name>
    <message>
        <location filename="maskpolywidget.ui" line="14"/>
        <source>Form</source>
        <translatorcomment>多边形标注界面</translatorcomment>
        <translation>多边形标注界面</translation>
    </message>
    <message>
        <location filename="maskpolywidget.ui" line="26"/>
        <source>Pre img</source>
        <translatorcomment>单击来切换上一张图片</translatorcomment>
        <translation>上一张图片</translation>
    </message>
    <message>
        <location filename="maskpolywidget.ui" line="39"/>
        <source>Next img</source>
        <translatorcomment>单击来切换下一张图片</translatorcomment>
        <translation>下一张图片</translation>
    </message>
    <message>
        <location filename="maskpolywidget.ui" line="52"/>
        <source>Open dir</source>
        <translatorcomment>单击来选择图片所在目录</translatorcomment>
        <translation>打开目录</translation>
    </message>
    <message>
        <location filename="maskpolywidget.ui" line="65"/>
        <source>Edit label</source>
        <translatorcomment>单击来编辑标签</translatorcomment>
        <translation>编辑标签</translation>
    </message>
    <message>
        <location filename="maskpolywidget.ui" line="78"/>
        <source>Label:</source>
        <translatorcomment>标签</translatorcomment>
        <translation>标签：</translation>
    </message>
    <message>
        <location filename="maskpolywidget.ui" line="122"/>
        <source>Label input:</source>
        <translatorcomment>输入标签</translatorcomment>
        <translation>输入标签：</translation>
    </message>
</context>
<context>
    <name>ProgressBarWidget</name>
    <message>
        <location filename="../MaskConfig/WIdgets/progressbarwidget.ui" line="14"/>
        <source>Form</source>
        <translatorcomment>多边形标注界面</translatorcomment>
        <translation>多边形标注界面</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/progressbarwidget.ui" line="35"/>
        <source>%p%</source>
        <translatorcomment>%p%</translatorcomment>
        <translation>%p%</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/progressbarwidget.ui" line="48"/>
        <source>Message Label</source>
        <translatorcomment>信息签</translatorcomment>
        <translation>信息签</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/progressbarwidget.ui" line="61"/>
        <source>Now at :</source>
        <translatorcomment>现位于：</translatorcomment>
        <translation>现位于：</translation>
    </message>
    <message>
        <location filename="../MaskConfig/WIdgets/progressbarwidget.ui" line="84"/>
        <source>Change to</source>
        <translatorcomment>单击来跳转</translatorcomment>
        <translation>跳转</translation>
    </message>
</context>
</TS>
