<?php
if (isset($_SERVER['HTTP_UPLOADDIR']))
{
    // for webserv
    echo "<pre>uploading from webserv\n</pre>";
    $uploadfile = $_SERVER['HTTP_UPLOADDIR'] . basename($_FILES['image']['name']);
}
else
{
    // for nginx
    echo "<pre>uploading from nginx\n</pre>";
    $uploaddir = "/home/kisobe/webserv/test/uploaded_images/";
    $uploadfile = $uploaddir . basename($_FILES['image']['name']);
}

echo '<pre>';
if (move_uploaded_file($_FILES['image']['tmp_name'], $uploadfile)) {
    echo "File is valid, and was successfully uploaded.\n";
} else {
    echo "Possible file upload attack!\n";
}

echo 'Here is some more debugging info:';
print_r($_FILES);

print "</pre>";
?>
