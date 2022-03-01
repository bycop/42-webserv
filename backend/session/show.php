<?php
session_start();

if (isset($_SESSION['coucou']))
    echo ($_SESSION['coucou']);
else
    echo ("Your session doesnt exist");
$_SESSION['coucou'] = 'blabla';
//echo '<pre>';
//var_dump($_SESSION);
//echo '</pre>';