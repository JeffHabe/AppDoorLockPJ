<?php
  
  $doorstatus = $_GET['status'];
  $btnstatus = $_GET['btnstatus'];


  //user information
  $host = "140.120.101.95";
  $user = "test123";
  $pass = "test123";

  //database information
  $databaseName = "iot";
  $tableName = "door";


  //Connect to mysql database
  $con = mysql_connect($host,$user,$pass);
  $dbs = mysql_select_db($databaseName, $con);


  //Query database for data
    $result = mysql_query("insert into $tableName (status,btn) VALUES ($doorstatus,$btnstatus)");

  //store matrix
  if($result==1)
    echo "success";
  else
    echo "error";
?>