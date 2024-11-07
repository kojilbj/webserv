<?php
session_start();
session_unset();
session_destroy();
setcookie("PHPSESSID", "", 0, "/");
echo "<a href='/index.html'>Go To Top Page</a>";
?>

