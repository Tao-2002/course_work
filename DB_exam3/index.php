<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>管理系统</title>
</head>
<style type="text/css">
.wrapper {
	/* width:1500px; */
	margin: 10px;
	border-radius:10px；
}
h3 {
	text-align: center;
}
.left-box{
	/* display:table; */
	width:50%;
	float:left;
}
.right-box{
	/* display:table; */
	width:50%;
	float:right;
}
.add {
	margin: 10px 0;
	text-align:center;
}
.add a {text-decoration: none;color: #fff;background-color:blue;padding: 6px;border-radius: 10px;}
td {
	text-align: center;
	border-radius:5px;
}
.table{
	width:400;
	background: blue;
}
table{
	/* border-collapse: collapse; */
	margin: 0 auto;
	text-align: center;
}
table td, table th{
	border: 3px solid #cad9ea;
	color: #666;
	height: 25px;
}
table thead th{
	background-color: #CCE8EB;
	width: 100px;
}

</style>
<body>
	<div class="wrapper">

		<?php
			// 导入配置文件
			require "dbconfig.php";
			// 连接mysql数据库
			$conn = @mysqli_connect(HOST,USER,PASS,DBNAME) or die("提示：数据库连接失败！");
			mysqli_select_db($conn,DBNAME);
			mysqli_set_charset($conn,'utf8');
			$tableName=['customers','employees','products','suppliers','purchases','logs'];
			$k=0;
			for($k=0;$k<6;$k++){
				if($k%2==0){
					echo"<div class='left-box'>";
				}else if($k%2==1){
					echo"<div class='right-box'>";
				}
				echo"<h3>{$tableName[$k]}管理系统</h3>
				<table id={$k} border=3>";

				$sql = "select * from {$tableName[$k]}";
				$result = mysqli_query($conn,$sql);
				
				$fieldNum=mysqli_num_fields($result);
				echo"<tr>";
				for($i=0;$i<$fieldNum;$i++){
					$field_info = mysqli_fetch_field_direct($result,$i);
					echo"<th>{$field_info->name}</th>";
				}
				echo"</tr>";

				// 解析
				$rowNum=mysqli_num_rows($result);  
				$idName=mysqli_fetch_field_direct($result,0);
				for($i=0; $i<$rowNum; $i++){
					$row = mysqli_fetch_array($result,MYSQLI_NUM);
					echo "<tr>";
					for($j=0;$j<$fieldNum;$j++){
						echo "<td>{$row[$j]}</td>";
					}
					echo "<td>
							<a href='javascript:del(\"{$row[0]}\",\"{$tableName[$k]}\",\"{$idName->name}\")'>删除</a>
							<a href='javascript:edit($k,$i )'>修改</a>
						</td>";
					echo "</tr>";
				}
				echo"</table>
					<div class='add' id='add-{$k}'>
						<a href='javascript:add($k);'>添加</a>
					</div>
				</div>";
			}
			mysqli_free_result($result);
		?>

	<script type="text/javascript">
		function add(id){
			var colNum=document.getElementById(id).rows[0].cells.length
			var tr=document.createElement("tr")
			for(let i=0;i<colNum;++i){
				var newL=document.createElement("td")
				newL.style.color='red'

				newL.contentEditable=true
				tr.appendChild(newL)
			} 
			var tr_a=document.createElement("td")
			var a_link_confirm=document.createElement('a')
			var a_link_cancel=document.createElement('a')

			a_link_confirm.href="javascript:post_message("+id+","+Number(-1)+")"
			a_link_confirm.innerHTML='确定'
			a_link_confirm.style.paddingRight='3px'
			a_link_cancel.href="javascript:cancel_add("+id+")"
			a_link_cancel.innerHTML='取消'
			var table=document.getElementById(id);
			tr_a.appendChild(a_link_confirm)
			tr_a.appendChild(a_link_cancel)
			tr.appendChild(tr_a)
			table.appendChild(tr)
			
			//禁用添加按钮
			var addButton=document.getElementById('add-'+id)
			addButton.style.pointerEvents='none'
		}

		function post_message(id,rowIndex){
			var table=document.getElementById(id);
			if(rowIndex==-1){
				var rowNum=table.rows.length-1
			}else{
				var rowNum=rowIndex+1
			}

			var colNum=table.rows[0].cells.length
			// 创建一个 form  
			var form = document.createElement("form");  
			// form.name="id";  
			// 添加到 body 中  
			document.body.appendChild(form);
			var idPost=document.createElement("input")
			idPost.type="text"
			idPost.name="id"
			idPost.value=id
			form.appendChild(idPost)
			// 创建一个输入  
			for(let i=0;i<colNum;i++){
				var input = document.createElement("input")
				// 设置相应参数  
				input.type = "text";  
				input.name = "value"+i;  
				// var mes=document.getElementById(i).
				input.value = table.rows[rowNum].cells[i].innerHTML
				console.log(input.value)
				// 将该输入框插入到 form 中  
				form.appendChild(input);
			}
			if(rowIndex==-1){
				//form 提交路径 
				form.action = "add.php" 
				//解除禁用
				var addButton=document.getElementById('add-'+id)
				addButton.style.pointerEvents=''
			}else{
				form.action ="edit.php"
			}
			// form 的提交方式  
			form.method = "POST" 
			//对该 form 执行提交  
			form.submit();  
			//删除该 form  
			document.body.removeChild(form)
		}

		function edit(tableId,rowIndex){
			var direct_row=document.getElementById(tableId).rows[rowIndex+1]

			var length=direct_row.cells.length-1
			for(let i=1;i<length;i++){
				direct_row.cells[i].contentEditable=true
			}
			direct_row.cells[length].children[0].style.display='none'
			direct_row.cells[length].children[1].style.display='none'
			var confirm=document.createElement('a')
			confirm.href="javascript:post_message("+tableId+","+rowIndex+")"
			confirm.innerHTML='确定'
			confirm.style.paddingRight='3px'
			var cancel=document.createElement('a')
			cancel.href="javascript:location.reload()"
			cancel.innerHTML='取消'
			direct_row.cells[length].appendChild(confirm)
			direct_row.cells[length].appendChild(cancel)
		}

		function cancel_add(id){
			var deltr=document.getElementById(id).rows
			var delIndex=deltr.length-1
			deltr[delIndex].remove()
			var addButton=document.getElementById('add-'+id)
			addButton.style.pointerEvents='auto'
		}

		function del(id,tableName,idName) {
			if (confirm("确定删除这条信息吗?")){
				window.location = "del.php?id="+id+"&tableName="+tableName+"&idName="+idName;
			}
		}
	</script>
</body>
</html>
