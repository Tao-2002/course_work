<?php
// 处理编辑操作的页面 
require "dbconfig.php";
// 连接mysql
$conn = @mysqli_connect(HOST,USER,PASS,DBNAME) or die("提示：数据库连接失败！");
mysqli_select_db($conn,DBNAME);
mysqli_set_charset($conn,'utf8');

$tableName=['customers','employees','products','suppliers','purchases','logs'];
// 获取修改的新闻
$id = $_POST['id'];
$sql = "select * from {$tableName[$id]}";
$result = mysqli_query($conn,$sql);
$fieldNum=mysqli_num_fields($result);
$valArr[$fieldNum]=[];
for($i=0;$i<$fieldNum;$i++){
    $valArr[$i]=$_POST["value{$i}"];
    echo "{$valArr[$i]}   ";
}
// 更新数据
$sqli="UPDATE {$tableName[$id]} SET ";
for($i=1;$i<$fieldNum;$i++){
    $field_info = mysqli_fetch_field_direct($result,$i);
    if($i!=$fieldNum-1){
        $sqli.="$field_info->name='{$valArr[$i]}',";
    }else{
        $idName=mysqli_fetch_field_direct($result,0);
        $sqli.="$field_info->name='{$valArr[$i]}'  WHERE {$idName->name} ='{$valArr[0]}'";
    }
}
mysqli_query($conn,$sqli) or die('修改数据出错：'.mysqli_error($conn)); 
mysqli_query($conn,"INSERT INTO LOGS (who,time,table_name,operation,key_value) VALUES ('super',now(),\"{$tableName[$id]}\" ,'edit',0)") or die('插入log数据出错:'.mysqli_error($conn));
header("Location:index.php");  

