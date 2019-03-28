<?php

      $lockstatus = $_GET['status'];
   	//user information
   	$host = "140.120.101.95";
   	$user = "test123";
   	$pass = "test123";

   	//database information
   	$databaseName = "iot";
   	$tableName = "locker";

   	//Connect to mysql database
   	$con = mysql_connect($host,$user,$pass);
   	$dbs = mysql_select_db($databaseName, $con);

    //Query database for data
   	$result=mysql_query("UPDATE  $tableName SET $tableName.`status` =$lockstatus  where id = 1 "); //check 目前狀態是關or開

      if($result==1)
       echo "success";
      else
       echo "error";
?>