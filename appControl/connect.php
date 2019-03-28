<?php
	// define("DB_HOST", "140.120.101.95:3306");
 //  define("DB_USERNAME", "test123");
 //  define("DB_PASSWORD", "test123");
 //  define("DB_DATABASE", "iot");
  include('config.php');

  	$db = new mysqli(DB_HOST,DB_USERNAME,DB_PASSWORD,DB_DATABASE);
  	if (!$db) {
  		echo "Error: Unable to connect to MySQL." . PHP_EOL;
  		echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
  		echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
  		exit;
  	}
	// echo "Host information: " . mysqli_get_host_info($db) . PHP_EOL;
?>