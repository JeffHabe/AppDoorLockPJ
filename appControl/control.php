<?php
  include('connect.php');

  if($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["lockStatus"])){
    $status = $db->real_escape_string($_POST["lockStatus"]);

    $db->query("UPDATE locker SET locker.`status` = '$status' WHERE id = 1;");

    echo "success";
  }

 ?>
