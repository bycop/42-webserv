<?php
session_start();
echo "<h1>Hello !</h1>";
echo "<p>".
        "<a href='create.php'>Create a session variable</a>" . "<br>" .
        "<a href='delete.php'>Delete  a session variable</a>" . "<br>" .
        "<a href='show.php'>Show a session variable</a>" .
"</p>";
