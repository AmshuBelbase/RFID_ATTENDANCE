<?php
if (!isset($_SESSION)) {
    session_start();
}
if ((!isset($_SESSION["email"])) || (!isset($_SESSION["desig"]))) {
    echo "<script>window.location.href ='index.php';</script>";
}
if ($_SESSION["desig"] != "ADMIN") {
    echo "<script>window.location.href ='index.php';</script>";
}
date_default_timezone_set('Asia/Kathmandu');
?>

<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">
    <title>Remove User - RFID</title>
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
            apiKey: "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630",
            authDomain: "narcs-1805c.firebaseapp.com",
            databaseURL: "https://narcs-1805c.firebaseio.com",
            projectId: "narcs-1805c",
            storageBucket: "narcs-1805c.appspot.com",
            messagingSenderId: "83689842797",
            appId: "1:83689842797:web:c1cc6cf94f28b7dadf3764"
        };

        // Initialize Firebase
        const app = initializeApp(firebaseConfig);
        const auth = getAuth(app);
        const database = getDatabase(app);
        // const db = getFirestore(app);
        // var found = "true";

        var mono = '-';

        async function checkPhone() {
            let moval = document.getElementById('mono').value;
            const dbRef = ref(database, `rfid/users/`);
            // // Query by email property
            const quer = query(dbRef, orderByChild('phone'), equalTo(moval));
            // async query
            try {
                const snapshot = await get(quer);
                if (snapshot.exists()) {
                    return false;
                } else {
                    document.getElementById('mospan').innerHTML = "❌ Phone Number doesn't exist";
                    document.getElementById('mospans').innerHTML = '';
                    return true;
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

        async function signup() {
            let checkPh = await checkPhone();
            if (checkPh == false) {
                mono = document.getElementById('mono').value;
                signInAnonymously(auth)
                    .then((userCredential) => {
                        //user.uid  
                        set(ref(database, 'rfid/users/' + mono), {
                            username: null,
                            address: null,
                            userreg: null,
                            phone: null,
                            email: null,
                            password: null,
                            rfid: null,
                            desig: null
                        }).then((success) => {
                            alert("User Removed Successfully");
                            window.location.href = 'index.php';

                        }).catch((error) => {
                            alert("User couldn't be Removed !!");
                        });
                    })
                    .catch((error) => {
                        var errorCode = error.code;
                        var errorMessage = error.message;
                        alert("Error Code - " + errorCode);
                        alert("Error Message - " + errorMessage);
                    });
            } else {
                console.log("No Phone");
            }
        }

        const conf = document.getElementById("submit");
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

                                <?php echo $_SESSION['desig'];
                                if ($_SESSION['desig'] == "ADMIN") {
                                    ?>
                                    </p>

                                    <a href="./dashboard.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Dash
                                        Board</a>
                                    <a href="./index.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Home</a>
                                    <a href="./permit_lab.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Permit LAB</a>
                                    <a href="./add_lab.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Add Lab</a>
                                    <a href="./all_users.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">All Users</a>
                                    <a href="./allow_lab_creation.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Make LAB ADMIN</a>
                                    <a href="./make_admin.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Make ADMIN</a>
                                    <a href="./approve.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Approve User</a>
                                <?php } ?>
                            </div>
                        </div>

                        <div class="login-wrap p-4 p-lg-5" id="mainsection">
                            <div class="d-flex">
                                <div class="w-100">
                                    <h3 class="mb-4">Remove User</h3>
                                </div>

                            </div>

                            <div class="form-group mb-3">
                                <label class="label" for="name">Phone (WhatsApp)*</label>
                                <span id="mospan" class="err"></span>
                                <span id="mospans" class="suc"></span>
                                <input type="number" class="form-control"
                                    placeholder="With Country Code eg. 91**********" id="mono" name="number"
                                    autocomplete="off" onchange="mova()">
                            </div>
                            <div class="form-group">
                                <span id="suberr" class="err"></span>

                                <button class="form-control btn btn-primary submit px-3" id="submit"
                                    name="submit">Remove</button>
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