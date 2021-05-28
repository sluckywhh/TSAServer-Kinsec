#!/bin/sh
TSAServerVersion="1.0.0"
TSASvrCltDllVersion="1.0.0"
rm -rf ../Bin ../Obj ../Lib
mkdir ../Obj
mkdir ../Lib
cp -r Bin ../
:<<!
echo "---------------------------------------------------------------"
echo "Preparing to create TSADbService files ......"
cd TSADbService
make -f TSADbService.mak
cd ..
!
echo "---------------------------------------------------------------"
echo "Preparing to create TSATimeSync files ......"
cd TSATimeSync
make -f TSATimeSync.mak
cd ..
:<<!
echo "---------------------------------------------------------------"
echo "Preparing to create TSAService files ......"
cd TSAService
make -f TSAService.mak
cd ..
!
:<<!
echo "---------------------------------------------------------------"
echo "Preparing to create TSAServer files ......"
cd TSAServer
make -f TSAServer.mak
cd ..
!
:<<!
echo "---------------------------------------------------------------"
echo "Preparing to create TSAMonitor files ......"
cd TSAMonitor
make -f TSAMonitor.mak
cd ..
!
echo "---------------------------------------------------------------"
echo "Preparing to create TSAAdmin files ......"
cd TSAAdmin
make -f TSAAdmin.mak
cd ..

echo "---------------------------------------------------------------"
echo "Preparing to create TSService files ......"
cd TSService
make -f TSService.mak
cd ..
:<<!
echo "---------------------------------------------------------------"
echo "Preparing to create sendmail files ......"
cd sendmail_
make -f sendmail.mak
cd ..
!
echo "---------------------------------------------------------------"
echo "Preparing to create TSASvrCltDll library ......"
cd TSASvrCltDll
make -f TSASvrCltDll.mak
cd ..

echo "---------------------------------------------------------------"
echo "Preparing to create TSASvrCltDll install files......"
cd ..
mkdir TSASvrCltDll
cd TSASvrCltDll
echo "#!/bin/sh" | cat >> install.sh
echo "echo \"Starting Clean Older Version...\"" | cat >> install.sh
echo "rm -f /usr/local/include/TSASvrCltDll.h" | cat >> install.sh
echo "rm -f /usr/local/lib/libTSASvrCltDll.so" | cat >> install.sh
echo "rm -f /lib/libTSASvrCltDll.so" | cat >> install.sh
echo "echo \"Starting Copy Files...\"" | cat >> install.sh
echo "cp ../Include/TSASvrCltDll.h /usr/local/include/" | cat >> install.sh
echo "cp ../Lib/libTSASvrCltDll.so /usr/local/lib/" | cat >> install.sh
echo "echo \"Starting Create Link Symbolic...\"" | cat >> install.sh
echo "ln -s /usr/local/lib/libTSASvrCltDll.so /lib/libTSASvrCltDll.so" | cat >> install.sh
echo "echo \"Install TSASvrCltDll Succession.\"" | cat >> install.sh
chmod +x install.sh
./install.sh
echo "---------------------------------------------------------------"
echo "Preparing to create TSASvrCltDll uninstall files......"
echo "#!/bin/sh" | cat >> uninstall.sh
echo "echo \"Starting unistall...\"" | cat >> uninstall.sh
echo "rm -f /usr/local/include/TSASvrCltDll.h" | cat >> uninstall.sh
echo "rm -f /usr/local/lib/libTSASvrCltDll.so" | cat >> uninstall.sh
echo "rm -f /lib/libTSASvrCltDll.so" | cat >> uninstall.sh
echo "echo \"Uninstall finish.\"" | cat >> uninstall.sh
echo "create TSASvrCltDll uninstall files Succession."
chmod +x uninstall.sh
cd ..
echo "---------------------------------------------------------------"
echo "Create install TSASvrCltDll package ......"
cp -rf Include Lib TSASvrCltDll
rm -f TSASvrCltDll/Include/TSErr.h
tar -zcvf TSASvrCltDll-RHEL6.5-1.0.0.tar.gz TSASvrCltDll
rm -rf TSASvrCltDll
echo "---------------------------------------------------------------"
echo "Create install TSAServer package ......"
cp -r Bin ./TSAServer
tar -zcvf TSAServer-RHEL6.5-1.0.0.tar.gz TSAServer
rm -rf TSAServer
cd Src
echo "---------------------------------------------------------------"
echo "Create Succession ==>"
echo "../TSASvrCltDll-RHEL6.5-"${TSASvrCltDllVersion}".tar.gz"
echo "../TSAServer-RHEL6.5-"${TSAServerVersion}".tar.gz"

