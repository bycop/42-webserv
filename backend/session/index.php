<?php
session_start();
echo "<h1 style='text-align: center'>Index</h1>";
if (isset($_SESSION['name'])) {
    echo '<hr>';
    echo '<div style="display: flex;flex-direction: column; justify-content: center;align-items: center">' .
            '<h2 style="font-weight: bold;text-align: center">' . $_SESSION['name'] . ' is register</h2>' .
            '<iframe src="https://giphy.com/embed/IwAZ6dvvvaTtdI8SD5" width="480" height="400" frameBorder="0" class="giphy-embed" allowFullScreen></iframe>' .
        '</div>';
    echo '<hr>';
}
echo '<h3>Links :</h3>';
echo "<p>".
        "<a href='create.php'>Create a session variable</a>" . "<br>" .
        "<a href='delete.php'>Delete  a session variable</a>" . "<br>" .
        "<a href='show.php'>Show a session variable</a>" .
"</p>";
