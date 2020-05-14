@set VERSION=1.1.1
@set ZIPTOOL="..\..\Tools\7-Zip\64\7z.exe"

echo 压缩安装包文件夹
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\Audio
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\config
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\fonts
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\images
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\Pre-Signal

echo 压缩安装包文件
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\ETCS_QML.exe
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\lame.exe
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\adpcm_converter.exe
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\Logic.dll
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\libConfig.dll
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip .\Bin\Protocol.dll

echo 压缩依赖文件
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip ..\..\Tools\QT-depend(x86)\*
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip ..\..\Tools\vc_redist2015\x86\*
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-Win.zip ..\..\Tools\vc_redist2015\Microsoft.VC140.CRT\*

echo 压缩PDB调试文件
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-PDB.zip .\Bin\libConfig.pdb
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-PDB.zip .\Bin\Protocol.pdb
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-PDB.zip .\Bin\Logic.pdb
%ZIPTOOL% a -tzip .\out\ETCS-%VERSION%-PDB.zip .\Bin\ETCS_QML.pdb

goto SUCCESS


::错误信息

:ERRORSLN
set ErrorMessage=编译解决方案sln失败
goto ERROR

:ERROR
start mshta vbscript:msgbox("%ErrorMessage%",,"错误!")(window.close)
goto END

:SUCCESS
start mshta vbscript:msgbox("恭喜你，打包成功",,"成功")(window.close)
:END

pause