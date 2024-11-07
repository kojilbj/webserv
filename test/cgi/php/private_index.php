<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>My Gallery</title>
    <style>
        body {
            background: linear-gradient(45deg, #f09433, #e6683c, #dc2743, #cc2366, #bc1888);
            font-family: 'Roboto', sans-serif;
            color: white;
            text-align: center;
            margin: 0;
            padding: 0;
        }

        .header {
            padding: 20px;
            background-color: rgba(0, 0, 0, 0.2);
            font-size: 24px;
        }

        .gallery {
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            padding: 20px;
        }

        .gallery img {
            width: 200px;
            height: 200px;
            margin: 10px;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }

        .upload-button {
            margin-top: 20px;
        }

        a {
            text-decoration: none;
            color: #fff;
            background-color: rgba(0, 0, 0, 0.3);
            padding: 10px 20px;
            border-radius: 10px;
        }
    </style>
</head>
<body>
	<?php
	session_start();
	if (!isset($_SESSION['username'])) {
        echo "Invalid credentials! <a href='/login.html'>Try again</a>";
		exit();
	}
	?>
    <header>
        <h1>ImageShare - My Gallery</h1>
        <div class="user-info">
            <span>Welcome, <?php echo $_SESSION['username']; ?>!</span>
            <a href="/php/logout.php" class="logout-btn">Log Out</a>
        </div>
    </header>

    <section class="gallery">
    </section>
</body>
</html>

