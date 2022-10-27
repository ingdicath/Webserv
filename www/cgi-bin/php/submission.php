<?php
if (!empty($_GET))
	echo $_GET["name"] . " picked " . $_GET["colour"] . " as their favourite color with GET method.";
else if (!empty($_POST))
	echo $_POST["name"] . " picked " . $_POST["colour"] . " as their favourite color with POST method.";

?>
