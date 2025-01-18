<?php
if (!isset($_SESSION)) {
    session_start();
}
if (isset($_SESSION["email"])) {
    echo "<script>window.location.href ='main.php';</script>";
}
date_default_timezone_set('Asia/Kathmandu');
?>

<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">
    <title>Forgot Password - RFID</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link href="https://fonts.googleapis.com/css?family=Lato:300,400,700,900&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="css/rfid.css">
    <link rel="stylesheet" href="css/addon.css">

    <script type="module">
        // Import the functions you need from the SDKs you need
        import { initializeApp } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-app.js";
        import { getAuth, signInAnonymously } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-auth.js";
        import {
            getDatabase,
            ref,
            set,
            child,
            equalTo,
            update,
            get,
            endAt,
            onValue,
            remove,
            query,
            onChildAdded,
            orderByChild,
        } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-database.js";
        // import { getFirestore, doc, getDoc } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-firestore.js";


        // TODO: Add SDKs for Firebase products that you want to use
        // https://firebase.google.com/docs/web/setup#available-libraries

        // Your web app's Firebase configuration
        const firebaseConfig = {
            apiKey: "api key",
            authDomain: "authDomain",
            databaseURL: "databaseURL",
            projectId: "projectId",
            storageBucket: "storageBucket",
            messagingSenderId: "messagingSenderId",
            appId: "appId"
        };

        // Initialize Firebase
        const app = initializeApp(firebaseConfig);
        const auth = getAuth(app);
        const database = getDatabase(app);
        // const db = getFirestore(app);
        // var found = "true";


        async function calculateHash(inputText) {

            // Convert the input string to ArrayBuffer
            const encoder = new TextEncoder();
            const dataen = encoder.encode(inputText);

            // Calculate SHA-256 hash
            const buffer = await crypto.subtle.digest("SHA-256", dataen);

            // Convert the hash to a hexadecimal string
            const hashArray = Array.from(new Uint8Array(buffer));
            const hashHex = hashArray.map(byte => byte.toString(16).padStart(2, "0")).join("");

            // Display the hash result
            return hashHex;
        }

        var moval = '';
        var movalemail = '';
        var emval = '';
        var psdval = '';
        var cpsdval = '';
        var code = '';

        async function checkPhone() {
            movalemail = '';
            moval = document.getElementById('mono').value;
            const dbRef = ref(database, `rfid/users/`);
            // // Query by email property
            const quer = query(dbRef, orderByChild('phone'), equalTo(moval));
            // async query
            try {
                const snapshot = await get(quer);
                if (snapshot.exists()) {
                    const dataSnap = snapshot.val();
                    movalemail = dataSnap[moval].email;
                    // console.log(dataSnap[moval].email);
                    return true;
                } else {
                    document.getElementById('mospan').innerHTML = '❌ Phone Number does not exists';
                    document.getElementById('mospans').innerHTML = '';
                    return false;
                }
            } catch (error) {
                console.error(error);
                return false;
            }
        }
        const movalidate = document.getElementById("mono");
        if (movalidate != null) {
            movalidate.addEventListener("change", checkPhone);
        }
        async function checkEmail() {
            emval = document.getElementById('email').value;
            const dbRe = ref(database, `rfid/users/`);

            // // Query by email property
            const que = query(dbRe, orderByChild('email'), equalTo(emval));
            try {
                const snapshot = await get(que);
                if (snapshot.exists()) {
                    return true;
                } else {
                    document.getElementById('emailspan').innerHTML = '❌ Email does not exists';
                    document.getElementById('emailspans').innerHTML = '';
                    return false;
                }
            } catch (error) {
                console.error(error);
                return false;
            }
        }
        const emvalidate = document.getElementById("email");
        if (emvalidate != null) {
            emvalidate.addEventListener("change", checkEmail);
        }

        async function sendOTP() {
            if (subvai() == true) {
                let checkEm = await checkEmail();
                let checkPh = await checkPhone();
                // console.log("checkEm - " + checkEm + typeof checkEm);
                // console.log("checkPh - " + checkPh + typeof checkPh);
                if (checkEm == true) {
                    if (checkPh == true) {
                        emval = document.getElementById('email').value;
                        if (movalemail == emval) {
                            const min = 10000000;
                            const max = 99999999;
                            const randomCode = Math.floor(Math.random() * (max - min + 1)) + min;
                            code = randomCode.toString();
                            document.getElementById("dis").style.display = "block";
                            let xhr = new XMLHttpRequest();
                            xhr.open("POST", "send_mail.php", true);
                            xhr.onload = () => {
                                if (xhr.readyState === XMLHttpRequest.DONE) {
                                    if (xhr.status === 200) {
                                        let data = xhr.response;
                                        if (data == "sent") {
                                            document.getElementById("otpsection").style.display = "block";
                                            document.getElementById("mainsection").style.display = "none";
                                        }
                                        else {
                                            document.getElementById("dis").style.display = "none";
                                        }
                                    }
                                }
                            }
                            xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                            xhr.send("code=" + code + "&email=" + emval);
                        }
                        else {
                            document.getElementById("suberr").innerHTML = "❌ Invalid Mobile and Email Pair.";
                        }
                    }
                    else {
                        console.log('No Match - Phone');
                    }
                }
                else {
                    console.log('No Match - Email');
                }
            }
        }

        async function signup() {

            if (otpva() == true) {
                // console.log("reached 2");
                var enteredOTP = document.getElementById("ono").value;
                if (code == enteredOTP) {
                    emval = document.getElementById('email').value;
                    moval = document.getElementById('mono').value;
                    psdval = document.getElementById('password').value;
                    psdval = await calculateHash(psdval);
                    // console.log("SHA-256 Hash: " + psdval);

                    // console.log(psdval);
                    signInAnonymously(auth)
                        .then((userCredential) => {
                            //user.uid  
                            update(ref(database, 'rfid/users/' + moval), {
                                password: psdval,
                            }).then(() => {
                                alert("Password Updated Successfully");
                                window.location.href = 'index.php';
                            }).catch((error) => {
                                alert("Password Update Failed: " + error.message);
                            });
                        })
                        .catch((error) => {
                            var errorCode = error.code;
                            var errorMessage = error.message;
                            alert("Error Code - " + errorCode);
                            alert("Error Message - " + errorMessage);
                        });
                }
                else {
                    alert("Unequal OTP");
                }
            }
        }
        const button = document.getElementById("submit");
        if (button != null) {
            button.addEventListener("click", sendOTP);
        }

        const conf = document.getElementById("confirm");
        if (conf != null) {
            conf.addEventListener("click", signup);
        }
    </script>
    <script src="js/rfid.js"></script>

</head>
<style>

</style>

<body>


    <section class="ftco-section">
        <div class="container">
            <div class="row justify-content-center">
                <div class="col-md-12 col-lg-10">
                    <div class="wrap d-md-flex">
                        <div class="text-wrap p-4 p-lg-5 text-center d-flex align-items-center order-md-last">
                            <div class="text w-100">
                                <h2>RFID Attendance</h2>
                                <p>Already have an account?</p>

                                <a href="./" class="btn btn-white btn-outline-white">Log In</a>
                            </div>
                        </div>
                        <div class="login-wrap p-4 p-lg-5 otpsection" id="otpsection">
                            <div class="d-flex">
                                <div class="w-100">
                                    <h3 class="mb-4">OTP</h3>
                                </div>

                            </div>

                            <span class="suc">Please check your Gmail Spam folder if it's not in your inbox.</span>

                            <div class="form-group mb-3">
                                <label class="label" for="name">Confirm OTP</label>
                                <input type="number" class="form-control" placeholder="OTP Number" id="ono" name="ono"
                                    max="99999999" min="10000000" autocomplete="off" onchange="ova()">
                                <span id="ospan" class="err"></span>
                                <span id="ospans" class="suc"></span>
                            </div>


                            <div class="form-group">
                                <!-- <input type="submit" class="form-control btn btn-primary submit px-3"
                                        value="Confirm OTP" id="confirm" name="confirm"> -->
                                <button class="form-control btn btn-primary submit px-3" id="confirm"
                                    name="confirm">Confirm OTP</button>
                                <span id="otperr" class="err"></span>
                            </div>
                        </div>

                        <div class="login-wrap p-4 p-lg-5" id="mainsection">
                            <div class="d-flex">
                                <div class="w-100">
                                    <h3 class="mb-4">Recover Password</h3>
                                </div>

                            </div>


                            <div class="form-group mb-3">
                                <label class="label" for="name">Phone (WhatsApp)*</label>
                                <input type="number" class="form-control"
                                    placeholder="With Country Code eg. 91**********" id="mono" name="number"
                                    autocomplete="off" onchange="mova()">
                                <span id="mospan" class="err"></span>
                                <span id="mospans" class="suc"></span>
                            </div>
                            <div class="form-group mb-3">
                                <label class="label" for="name">Email *</label>
                                <input type="email" class="form-control" placeholder="Email (To send OTP)" id="email"
                                    name="email" onkeyup="charOnly(this)" maxlength="35" autocomplete="off"
                                    onchange="emva()">
                                <span id="emailspan" class="err"></span>
                                <span id="emailspans" class="suc"></span>
                            </div>
                            <div class="form-group mb-3">
                                <label class="label" for="password">Password *</label>
                                <input type="password" class="form-control" placeholder="Password" id="password"
                                    name="password" onkeyup="charOnly(this)" maxlength="25" autocomplete="off"
                                    onchange="pdva()">
                                <span id="psdspan" class="err"></span>
                                <span id="psdspans" class="suc"></span>
                            </div>
                            <div class="form-group mb-3">
                                <label class="label" for="password">Confirm Password *</label>
                                <input type="password" class="form-control" placeholder="Confirm Password"
                                    id="cpassword" name="cpassword" onkeyup="charOnly(this)" maxlength="25"
                                    autocomplete="off" onchange="cpdva()">
                                <span id="cpsdspan" class="err"></span>
                                <span id="cpsdspans" class="suc"></span>
                            </div>
                            <div class="form-group">
                                <button class="form-control btn btn-primary submit px-3" id="submit"
                                    name="submit">Change Password</button>
                                <!-- <input type="submit" class="form-control btn btn-primary submit px-3" value="Sign Up"
                                    id="submit" name="submit"> -->
                                <span id="suberr" class="err"></span>
                                <span id="dis" class="suc">Navigating to the "Enter OTP" page...</span>

                            </div>
                            <div class="w-100 text-md-right">
                                * Required
                            </div>
                        </div>

                    </div>
                </div>
            </div>
        </div>
    </section>

</body>

</html>