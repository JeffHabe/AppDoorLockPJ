<?php
	include('connect.php');
	
	$query = "SELECT door.time,door.status,door.btn FROM door"; 
	$num = 0; 
	 if ($result = $db->query($query)) {
	 	while($object = $result->fetch_object()){
	 		$eachdata[$num] = $object;
	 		$num++;
	 	}
	 }
	 printf("{content: %s}",json_encode($eachdata));
?>