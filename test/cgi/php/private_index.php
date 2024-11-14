<!DOCTYPE html>
<html>
<head>
	<title>My Page</title>
</head>
<body>
	<h1>My Page</h1>
	<?php
	session_start();
	if (!isset($_SESSION['username'])) {
        echo "Invalid credentials! <a href='/login.html'>Try again</a>";
		exit();
	}
	?>
	<h2>Hello, <?php echo $_SESSION['username']; ?>!</h2>
	<p><a href="/php/logout.php">Log Out?</p>
</body>
</html>

