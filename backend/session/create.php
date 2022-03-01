<?php
session_start();
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    echo "<h1>Create a session variable</h1>";
    echo "<form action='#' method='post'" .
            "<label for='name'>Enter your name: " .
            "<input type='text' name='name'>". "<br>". "<br>".
            "<input type='submit' value='Submit'>".
         "</form>";
} else if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (!isset($_POST['name']) || $_POST['name'] == '') {
        echo "<h1>Error form. Please try again :</h1>";
        echo '<div style="width:480px"><iframe allow="fullscreen" frameBorder="0" height="480" src="https://giphy.com/embed/lkibHaGO1xmJXapEdq/video" width="480"></iframe></div>';
        echo "<a href='create.php'>Create a session variable</a>";
    } else {
        if (isset($_SESSION['name']))
            echo "<h1>Your name has been modify</h1>";
        else
            echo "<h1>Your name has been register</h1>";
        $_SESSION["name"] = $_POST["name"];
        echo "<p>Your name is : " . $_SESSION['name'] . "</p>";
        echo "<p>Go to <a href='index.php'>index</a> to see a surprise....</p>";
    }
}
?>