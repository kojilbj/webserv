<?php
if (isset($_POST['submit'])) {
    $username = htmlspecialchars($_POST['username']);
    $password = password_hash($_POST['password'], PASSWORD_BCRYPT);
    
    $file = fopen("users.csv", "a");
    fputcsv($file, [$username, $password]);
    fclose($file);
    
    echo "Registration successful! <a href='/login.html'>Log In</a>";
}
else {
	echo "Invalid Method";
}
?>
