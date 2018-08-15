BackendDevelopmentEnvironment

==============================

linux环境下一站式后台开发环境
基于该项目可以快速开发业务处理系统（c/c++ 或 python）和轻量级web站点，高性能高并发的后台server（tcp/http）或微服务
linux backend development environment, include glog,gtest,treeconf,protobuf,net.etc

=============================
How to use for a bare machine

第一步：安装git
sudo yum install git
git clone https://github.com/caszhang/BackendDevelopmentEnvironment.git
cd BackendDevelopmentEnvironment
git pull origin master
ssh-keygen -t rsa -C "80176975@qq.com"
cat ~/.ssh/id_rsa.pub
https://help.github.com/articles/generating-ssh-keys/#platform-linux

第二步：安装g++
yum list gcc-c++
sudo yum install gcc-c++.x86_64

第三步：安装zlib
yum install zlib
yum install zlib-devel

第四步：安装rz,sz
wget --no-check-certificate https://ohse.de/uwe/releases/lrzsz-0.12.20.tar.gz
tar -xzvf lrzsz-0.12.20.tar.gz 
cd lrzsz-0.12.20
sudo ./configure
sudo make
sudo make install
cd /usr/bin
ln -s /usr/local/bin/lrz rz
ln -s /usr/local/bin/lsz sz

第五步：安装python
tar -xzvf Python-2.7.10.tgz
cd Python-2.7.10
sudo ./configure
sudo make
sudo make install
python -V

第六步：安装mysql环境
sudo yum install mysql
sudo yum install mysql-devel

第七步：安装zip ,unzip
sudo yum install -y unzip zip

第七步：安装setuptools
wget https://bootstrap.pypa.io/ez_setup.py -O - | sudo python
unzip setuptools-18.0.1.zip
cd setuptools-18.0.1
sudo python setup.py build
sudo python setup.py install 

第八步：安装MySQLdb
tar -zxvf MySQL-python-1.2.3.tar.gz 
cd MySQL-python-1.2.3
sudo python setup.py build
sudo python setup.py install

第九步：安装tornado
 vi ~/.bashrc 
export PYTHONPATH=$PYTHONPATH:/home/guoqiang/github/BackendDevelopmentEnvironment/thirdparty/backports.ssl_match_hostname-3.4.0.2/src
export PYTHONPATH=$PYTHONPATH:/home/guoqiang/github/BackendDevelopmentEnvironment/thirdparty/tornado-3.2
exit
cd Python-2.7.10
sudo ./configure --with-ssl
sudo make
sudo make install
python -V

第十步：安装nginx
cd nginx-1.4.4/sbin
./nginx -V
cd /home
sudo mkdir users
sudo ln -s /home/guoqiang zhangguoqiang01


