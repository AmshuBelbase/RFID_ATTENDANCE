<?php
if (isset($_POST['email']) && isset($_POST['phone']) && isset($_POST['rfid']) && isset($_POST['userreg']) && isset($_POST['username']) && isset($_POST['desig'])) {
    $_SESSION['email'] = $_POST['email'];
    $_SESSION['phone'] = $_POST['phone'];
    $_SESSION['userreg'] = $_POST['userreg'];
    $_SESSION['desig'] = $_POST['desig'];
    $_SESSION['username'] = $_POST['username'];
    $_SESSION['rfid'] = $_POST['rfid'];

    echo ($_SESSION['email'] . "," . $_SESSION['phone'] . "," . $_SESSION['userreg'] . "," . $_SESSION['username'] . "," . $_SESSION['rfid'] . "," . $_SESSION['desig']);
}
?>