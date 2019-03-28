<?php 
	include ('connect.php');

	$status = "";
	//檢查username or password is set from android 

	  if($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["username"]) && isset($_POST["password"])){
		//innitialize variable
		$myusername = $db->real_escape_string($_POST["username"]);
		$mypassword = $db->real_escape_string($_POST["password"]);

		//Query database for row exist or not 
		$result =  $db->query("SELECT * FROM tbl_login WHERE id = '$myusername' AND pwd = '$mypassword'");

		$count = $result->num_rows;

		if ($count == 1) {
			$status ="true";
		}else
		{
			$status ="false";		
		}
		echo $status;
	}

?>