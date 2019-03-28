<?php
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
   	$search=mysql_query("SELECT status FROM $tableName where id = 1 "); //check 目前狀態是關or開
   	$result=mysql_fetch_row($search);

   	//echo $result[0];
      // echo "^";//signal for start
   
      // while ($row = $result->fetch_assoc()) {
      //    echo $row['status'];//fetch the light status 0 or 1 
         
      // }
      // echo "!";//signal for end

      if ($result[0] == 1){
         echo "^1!";
      }else{
         echo "^0!";
      }
      
?>