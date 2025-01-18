<?php

include('smtp/PHPMailerAutoload.php');

if (isset($_POST['code']) && isset($_POST['email'])) {
    smtp_mailer($_POST['email'], 'OTP for RFID Registration', $_POST['code']);
}

function smtp_mailer($to, $subject, $msg)
{
    $mail = new PHPMailer();
    //$mail->SMTPDebug  = 3;
    $mail->IsSMTP();
    $mail->SMTPAuth = true;
    $mail->SMTPSecure = 'tls';
    $mail->Host = "smtp.gmail.com";
    $mail->Port = 587;
    $mail->IsHTML(true);
    $mail->CharSet = 'UTF-8';
    $mail->Username = "@gmail.com"; //gmail
    $mail->Password = ""; //psd
    $mail->SetFrom("@gmail.com"); //gmail
    $mail->Subject = $subject;
    $msgg = "Enter this OTP : $msg";
    $mail->Body = $msgg;
    $mail->AddAddress($to);
    $mail->SMTPOptions = array(
        'ssl' => array(
            'verify_peer' => false,
            'verify_peer_name' => false,
            'allow_self_signed' => false
        )
    );
    if (!$mail->Send()) {
        echo "not sent";
    } else {
        echo "sent";
    }
}
?>