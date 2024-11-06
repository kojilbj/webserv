<?php
session_start();

if (isset($_POST['submit'])) {
    $username = $_POST['username'];
    $password = $_POST['password'];
    
    $file = fopen("users.csv", "r");
    $isUserFound = false;
    
    while (($userData = fgetcsv($file)) !== false) {
        if ($userData[0] === $username && password_verify($password, $userData[1])) {
            $_SESSION['username'] = $username;
            setcookie("PHPSESSID", session_id(), time() + 3600, "/");
            header("Location: private_index.html");
            $isUserFound = true;
            break;
        }
    }
    fclose($file);

    if (!$isUserFound) {
        echo "Invalid credentials! <a href='login.html'>Try again</a>";
    }
	else
	{
		echo "<a href='/php/private_index.php'>Go To My Page</a>";
	}
}
?>
