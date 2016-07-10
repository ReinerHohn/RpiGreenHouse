<?php
$con = mysql_connect("localhost","root","root");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}

mysql_select_db("humidities", $con);

$result = mysql_query("SELECT * FROM humiditydata LIMIT 1,10;");

while($row = mysql_fetch_array($result)) {
  echo $row['dateandtime'] . "," . $row['humidity1']. ";";
}

mysql_close($con);
?>
