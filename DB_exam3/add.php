<?php
// 处理增加操作的页面 
require "dbconfig.php";
// 连接mysql
$conn = @mysqli_connect(HOST,USER,PASS,DBNAME) or die("提示：数据库连接失败！");
mysqli_select_db($conn,DBNAME);
mysqli_set_charset($conn,'utf8');

$tableName=['customers','employees','products','suppliers','purchases','logs'];
// 获取增加数据
$id=$_POST['id'];
$sql = "select * from {$tableName[$id]}";
$result = mysqli_query($conn,$sql);
$fieldNum=mysqli_num_fields($result);
$valArr[$fieldNum]=[];
for($i=0;$i<$fieldNum;$i++){
    $valArr[$i]=$_POST["value{$i}"];
}
// 插入数据
$sqli ="INSERT INTO {$tableName[$id]} VALUES ('";
for($i=0;$i<$fieldNum;$i++){
    if($i!=$fieldNum-1){
        $sqli.="{$valArr[$i]}','";
    }else{
        $sqli.="{$valArr[$i]}')";
    }
}
// echo "{$sqli}";
mysqli_query($conn,$sqli) or die('添加数据出错：'.mysqli_error($conn)); 
mysqli_query($conn,"INSERT INTO LOGS (who,time,table_name,operation,key_value) VALUES ('super',now(),\"{$tableName[$id]}\" ,'add',0)") or die('插入log数据出错:'.mysqli_error($conn));
header("Location:index.php");  

