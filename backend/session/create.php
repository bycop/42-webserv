<?php
session_start();
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    echo '<h1 style="text-align: center">Create</h1>';
    echo "<h2>Form:</h2>";
    echo "<form action='#' method='post'" .
            "<label for='name'>Enter your name: " .
            "<input type='text' name='name'>". "<br>". "<br>".
            "<input type='submit' value='Submit'>".
         "</form>";
} else if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (!isset($_POST['name']) || $_POST['name'] == '') {
        echo "<h1 style=\"text-align: center\">Error form. Please try again :</h1>";
        echo "<hr>";
        echo "<div style='display: flex;justify-content: center;align-items: center'>";
            echo '<div style="width:480px"><iframe allow="fullscreen" frameBorder="0" height="480" src="https://giphy.com/embed/lkibHaGO1xmJXapEdq/video" width="480"></iframe></div>';
        echo "</div>";
        echo "<hr>";
        echo "<h2>Links : </h2>";
        echo "<a href='create.php'>Create a session variable</a>";
    } else {
        if (isset($_SESSION['name']))
            echo "<h1>Your name has been modified</h1>";
        else
            echo "<h1>Your name has been registered</h1>";
        $_SESSION["name"] = $_POST["name"];
        echo "<p>Your name is : " . $_SESSION['name'] . "</p>";
        echo "<p>Go to <a href='index.php'>index</a> to see a surprise....</p>";
    }
}
?>