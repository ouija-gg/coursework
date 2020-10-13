#!/bin/sh 
#exec /opt/lampp/bin/mysql
dbstatus=`systemctl status | grep -owe 'Active: active'`
case $dbstatus in
    "Active: active")
        echo mariadb already running
        ;;
    *)
        systemctl start mariadb
esac
mysql -p

