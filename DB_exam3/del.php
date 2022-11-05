<?php
// 处理删除操作的页面 
require "dbconfig.php";
// 连接mysql
$conn = @mysqli_connect(HOST,USER,PASS,DBNAME) or die("提示：数据库连接失败！");
mysqli_select_db($conn,DBNAME);
mysqli_set_charset($conn,'utf8');
$id = $_GET['id'];
$tableName=$_GET['tableName'];
$idName=$_GET['idName'];
//删除指定数据  
mysqli_query($conn,"DELETE FROM {$tableName} WHERE {$idName}=\"{$id}\" ") or die('删除数据出错：'.mysqli_error($conn)); 
//向日志管理系统添加数据
if("$tableName"!="logs"){
    mysqli_query($conn,"INSERT INTO LOGS (who,time,table_name,operation,key_value) VALUES ('super',now(),\"{$tableName}\" ,'del',0)") or die('插入log数据出错:'.mysqli_error($conn));
}
header("Location:/exercise1/index.php");  


