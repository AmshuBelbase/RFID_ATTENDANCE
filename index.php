<?php
if (!isset($_SESSION)) {
    session_start();
}

if (isset($_SESSION["email"])) {
    echo "<script>window.location.href ='main.php';</script>";
}

date_default_timezone_set('Asia/Kolkata');
?>
<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">
    <title>Log In - RFID</title>
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

        async function checkLog() {
            var emval = document.getElementById('email').value;
            var psdval = document.getElementById('password').value;
            psdval = await calculateHash(psdval);

            const dbRe = ref(database, `rfid/users/`);
            let childNodeKey = false;
            // // Query by email property
            const que = query(dbRe, orderByChild('email'), equalTo(emval));
            try {
                const snapshot = await get(que);
                if (snapshot.exists()) {
                    snapshot.forEach((childSnapshot) => {
                        let childNode = childSnapshot.val();
                        if (childNode['password'] == psdval) {
                            childNodeKey = childSnapshot.key;
                            childNodeKey = childNode;
                            // console.log(`Child Node Key: ${childNodeKey}`);
                        }
                    });
                    return childNodeKey;
                } else {
                    document.getElementById('emailspan').innerHTML = '❌ Email does not exists';
                    document.getElementById('emailspans').innerHTML = '';
                    return childNodeKey;
                }
            } catch (error) {
                console.error(error);
                return childNodeKey;
            }
        }

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

        async function login() {
            if (sublog() == true) {
                let checkedVal = await checkLog();
                if (checkedVal != false) {
                    document.getElementById("iemail").innerHTML = "";
                    var email = checkedVal['email'];
                    var phone = checkedVal['phone'];
                    var username = checkedVal['username'];
                    var userreg = checkedVal['userreg'];
                    var rfid = checkedVal['rfid'];
                    var desig = checkedVal['desig'];

                    let xhr = new XMLHttpRequest();
                    xhr.open("POST", "setSession.php", true);
                    xhr.onload = () => {
                        if (xhr.readyState === XMLHttpRequest.DONE) {
                            if (xhr.status === 200) {
                                let data = "unset";
                                data = xhr.response;
                                if (data != "unset") {
                                    // console.log(data);
                                    window.location.href = 'main.php';
                                }
                            }
                        }
                    }
                    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                    xhr.send("email=" + email + "&phone=" + phone + "&username=" + username + "&userreg=" + userreg + "&rfid=" + rfid + "&desig=" + desig);
                }
                else {
                    document.getElementById("iemail").innerHTML = "Incorrect Email or Password ❌";
                }
            }
        }


        const button = document.getElementById("submit");
        if (button != null) {
            button.addEventListener("click", login);
        }
    </script>
    <script src="js/rfid.js"></script>

</head>

<body>
    <section class="ftco-section">
        <div class="container">
            <div class="row justify-content-center">
                <div class="col-md-12 col-lg-10">
                    <div class="wrap d-md-flex">
                        <div class="text-wrap p-4 p-lg-5 text-center d-flex align-items-center order-md-last">
                            <div class="text w-100">
                                <h2>RFID Attendance</h2>
                                <p>Don't have an account?</p>
                                <a href="signup.php" class="btn btn-white btn-outline-white">Sign Up</a>
                            </div>
                        </div>
                        <div class="login-wrap p-4 p-lg-5">
                            <div class="d-flex">
                                <div class="w-100">
                                    <h3 class="mb-4">Sign In</h3>
                                </div>

                            </div>

                            <span class="err" id="iemail"></span>


                            <div class="form-group mb-3">
                                <label class="label" for="name">Email</label>
                                <span id="emailspan" class="err"></span>
                                <span id="emailspans" class="suc"></span>
                                <input type="email" class="form-control" placeholder="Email (To send OTP)" id="email"
                                    name="email" onkeyup="charOnlyi(this)" maxlength="35" autocomplete="off"
                                    onchange="emva()">
                            </div>
                            <div class="form-group mb-3">
                                <label class="label" for="password">Password</label><span id="psdspan"
                                    class="err"></span>
                                <span id="psdspans" class="suc"></span>
                                <input type="password" class="form-control" placeholder="Password" id="password"
                                    name="password" onkeyup="charOnlyi(this)" maxlength="25" autocomplete="off"
                                    onchange="pdva()">

                            </div>
                            <div class="form-group">
                                <!-- <input type="submit" class="form-control btn btn-primary submit px-3" value="Log In"
                                        id="submit" name="submit"> -->
                                <button class="form-control btn btn-primary submit px-3" id="submit" name="submit">Log
                                    In</button>
                                <span id="suberr" class="err"></span>
                            </div>
                            <div class="form-group d-md-flex">

                                <div class="w-50 text-md-right">
                                    <a href="forgot_psd.php">Forgot Password</a>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
</body>

</html>