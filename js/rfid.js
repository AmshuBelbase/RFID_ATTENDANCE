// function mava(){
//     var sa = document.getElementById('selected_admin').value;
//     if (sa == '' || sa < 1){
//         document.getElementById('userspans').innerHTML = '';
//         document.getElementById('userspan').innerHTML = '❌ Please select a Member';
//         return false;
//     }
// }
// function ruva(){
//     var selected_user = document.getElementById('selected_user').value;
//     if (selected_user == '' || selected_user < 1){
//         document.getElementById('userspans').innerHTML = '';
//         document.getElementById('userspan').innerHTML = '❌ Please select a Member';
//         return false;
//     }
// }
// function asva(){ 
//     var sa = document.getElementById('savings').value;
//     var interest = document.getElementById('interest').value;
//     //var selected_users = document.getElementById('selected_admin').value; 
    
//     if ((sa == '') || (interest == '')){ 
//         if(sa == ''){
//             document.getElementById('savingspans').innerHTML = '';
//             document.getElementById('savingspan').innerHTML = '❌ Saving Amount cannot be blank';
//         }
//         else{
//             document.getElementById('savingspan').innerHTML = '';
//         }
//         // if(selected_users == ''){
//         //     document.getElementById('suspans').innerHTML = '';
//         //     document.getElementById('suspan').innerHTML = '❌ Please Select User(s)';
//         // }
//         // else{
//         //     document.getElementById('suspan').innerHTML = '';
//         // }
//         if(interest == ''){
//             document.getElementById('interestspans').innerHTML = '';
//             document.getElementById('interestspan').innerHTML = '❌ Interest Amount cannot be blank';
//         }
//         else{
//             document.getElementById('interestspan').innerHTML = '';
//         }
//         return false;
//     }
//     else{
//         document.getElementById('savingspan').innerHTML = '';
//         document.getElementById('interestspan').innerHTML = '';
//         // document.getElementById('suspan').innerHTML = '';
//     } 
// }

function sublog() {
  var emval = document.getElementById('email').value;
  var psdval = document.getElementById('password').value; 

  if ((emval == '') || (psdval == '') || (psdval.length > 25) || (emval.length > 50) || (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in')<= 0)) {
 
    if (psdval == '') {
      
      document.getElementById('psdspans').innerHTML = '';
      document.getElementById('psdspan').innerHTML = '❌ Password cannot be blank';
    }
    if ((psdval.length > 25)) {
      document.getElementById('psdspans').innerHTML = '';
      document.getElementById('psdspan').innerHTML = '❌ Password should be of less than 25 digits';
    }
     
    if ((emval.length > 50)) {
      document.getElementById('emailspans').innerHTML = '';
      document.getElementById('emailspan').innerHTML = '❌ Email should be of less than 50 digits';
    }
    if (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in') <=0) {
      document.getElementById('emailspans').innerHTML = '';
      document.getElementById('emailspan').innerHTML = '❌ Invalid Email';
    }
    if (emval == '') {
      document.getElementById('emailspan').innerHTML = '❌ Email cannot be blank';
    }
    document.getElementById('suberr').innerHTML = '❌ Invalid Data Submitted ';
    document.getElementById('suberr').style.background = "white";
    return false;
  }
  else{
    return true;
  }
}

function subvai() {
    var moval = document.getElementById('mono').value;  
    var emval = document.getElementById('email').value;
    var psdval = document.getElementById('password').value;
    var cpsdval = document.getElementById('cpassword').value;

    if ((psdval != cpsdval) || (emval == '') || (cpsdval == '') || (psdval == '') || (psdval.length > 25) || (cpsdval.length > 25) || (emval.length > 50) || (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in')<=0) || moval.length >13 || moval.length == 0 || moval.length <= 10) {

      if ((psdval != cpsdval)) {
            document.getElementById('cpsdspans').innerHTML = '';
            document.getElementById('cpsdspan').innerHTML = '❌ Passwords do not match';
      }
      if (psdval == '') {
        
        document.getElementById('psdspans').innerHTML = '';
        document.getElementById('psdspan').innerHTML = '❌ Password cannot be blank';
      }
      if ((psdval.length > 25)) {
        document.getElementById('psdspans').innerHTML = '';
        document.getElementById('psdspan').innerHTML = '❌ Password should be of less than 25 digits';
      }
      if (cpsdval == '') {
        document.getElementById('cpsdspan').innerHTML = '❌ Confirm Password cannot be blank';
      }
      if ((cpsdval.length > 25)) {
        document.getElementById('cpsdspans').innerHTML = '';
        document.getElementById('cpsdspan').innerHTML = '❌ Confirm Password should be of less than 25 digits';
      }
      if ((emval.length > 50)) {
        document.getElementById('emailspans').innerHTML = '';
        document.getElementById('emailspan').innerHTML = '❌ Email should be of less than 50 digits';
      }
      if (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in')<=0) {
        document.getElementById('emailspans').innerHTML = '';
        document.getElementById('emailspan').innerHTML = '❌ Invalid Email';
      }
      if (emval == '') {
        document.getElementById('emailspan').innerHTML = '❌ Email cannot be blank';
      }
      if (moval.length >13 || moval.length == 0 || moval.length <= 10) {
        document.getElementById('mospans').innerHTML = '';
        document.getElementById('mospan').innerHTML = '❌ Invalid Number';
      }
      document.getElementById('suberr').innerHTML = '❌ Invalid Data Submitted ';
      document.getElementById('suberr').style.background = "white";
      return false;
    }
    return true;
  }

//   function selper(input){
//     toggleBtn = document.getElementById("active");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active2");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active3");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active4");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active5");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active6");
//     toggleBtn.classList.remove("active");

//     id = document.getElementById('username').value;
//     let xhr1 = new XMLHttpRequest();
//     xhr1.open("POST", "php/get-email.php", true);
//     xhr1.onload = ()=>{
//       if(xhr1.readyState === XMLHttpRequest.DONE){
//           if(xhr1.status === 200){
//             let data = xhr1.response; 
//             document.getElementById('email').value = data;
//           }
//       }
//     }
//     xhr1.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     xhr1.send("incoming_id="+id);

//     let xhr2 = new XMLHttpRequest();
//     xhr2.open("POST", "php/get-phone.php", true);
//     xhr2.onload = ()=>{
//       if(xhr2.readyState === XMLHttpRequest.DONE){
//           if(xhr2.status === 200){
//             let data = xhr2.response; 
//             document.getElementById('mono').value = data;
//           }
//       }
//     }
//     xhr2.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     xhr2.send("incoming_id="+id);

//     // let xhr3 = new XMLHttpRequest();
//     // xhr3.open("POST", "php/get-loans.php", true);
//     // xhr3.onload = ()=>{
//     //   if(xhr3.readyState === XMLHttpRequest.DONE){
//     //       if(xhr3.status === 200){
//     //         let data = xhr3.response; 
//     //         document.getElementById('ai').innerHTML += data;
//     //       }
//     //   }
//     // }
//     // xhr3.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     // xhr3.send("incoming_id="+id);

//     seloan(input);
// }

// function seloan(e){
//     username = document.getElementById('username').value;
//     pdate = document.getElementById('pdate').value;
//     // alert(pdate);   
//     let xhr1 = new XMLHttpRequest();
//     xhr1.open("POST", "php/get-date.php", true);
//     xhr1.onload = ()=>{
//       if(xhr1.readyState === XMLHttpRequest.DONE){
//           if(xhr1.status === 200){
//             let data = xhr1.response; 
//             document.getElementById('todate').value = data;
//           }
//       }
//     }
//     loantype = 2;
//     xhr1.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     xhr1.send("ai="+username+"&loantype="+loantype+"&pdate="+pdate);
 
//     let xhr2 = new XMLHttpRequest();
//     xhr2.open("POST", "php/get-date.php", true);
//     xhr2.onload = ()=>{
//       if(xhr2.readyState === XMLHttpRequest.DONE){
//           if(xhr2.status === 200){
//             let data = xhr2.response; 
//             document.getElementById('la').value = data;
//           }
//       }
//     }
//     loantype = 1;
//     xhr2.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     xhr2.send("ai="+username+"&loantype="+loantype+"&pdate="+pdate);

     
// }

// function schng(input){
//     toggleBtn = document.getElementById("active");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active2");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active3");
//     toggleBtn.classList.remove("active");
//     toggleBtn = document.getElementById("active4");
//     toggleBtn.classList.remove("active");

//     id = document.getElementById('username').value;
//     let xhr1 = new XMLHttpRequest();
//     xhr1.open("POST", "php/get-email.php", true);
//     xhr1.onload = ()=>{
//       if(xhr1.readyState === XMLHttpRequest.DONE){
//           if(xhr1.status === 200){
//             let data = xhr1.response; 
//             document.getElementById('email').value = data;
//           }
//       }
//     }
//     xhr1.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     xhr1.send("incoming_id="+id);


//     let xhr2 = new XMLHttpRequest();
//     xhr2.open("POST", "php/get-phone.php", true);
//     xhr2.onload = ()=>{
//       if(xhr2.readyState === XMLHttpRequest.DONE){
//           if(xhr2.status === 200){
//             let data = xhr2.response; 
//             document.getElementById('mono').value = data;
//           }
//       }
//     }
//     xhr2.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//     xhr2.send("incoming_id="+id);
 
// }

function lettersOnly(input) {
    var regex = /[^ a-z]/gi;
    input.value = input.value.replace(regex, "");
}
function charOnly(input) {
    var regex = /[^ a-z 1234567890!@#$%^&*.]/gi;
    input.value = input.value.replace(regex, ""); 
}
function charOnlyreg(input) {
  var regex = /[^ a-z1234567890]/gi;
  input.value = input.value.replace(regex, ""); 
}
// function charOnlyo(input) {
//     var regex = /[^1234567890]/gi;
//     input.value = input.value.replace(regex, "");
//     document.getElementById("iemail").innerHTML = ''; 
// }
function charOnlyi(input) {
    var regex = /[^ a-z 1234567890!@#$%^&*.]/gi;
    input.value = input.value.replace(regex, "");
    document.getElementById("iemail").innerHTML = ''; 
}
function cpdva() {
    var psdval = document.getElementById('password').value;
    var cpsdval = document.getElementById('cpassword').value;
    if ((psdval != cpsdval)) {
        document.getElementById('cpsdspans').innerHTML = '';
        document.getElementById('cpsdspan').innerHTML = '❌ Passwords do not match';
    }
    if (psdval == cpsdval) {
        document.getElementById('cpsdspan').innerHTML = '';
        document.getElementById('cpsdspans').innerHTML = '✔ Password Confirmed';
    }
}
function pdva() {
    var psdval = document.getElementById('password').value;
    if ((psdval.length > 25)) {
        document.getElementById('psdspans').innerHTML = '';
        document.getElementById('psdspan').innerHTML = '❌ Password should be of less than 25 digits';
    }
    if ((psdval.length <= 25)) {
        document.getElementById('psdspan').innerHTML = '';
        document.getElementById('psdspans').innerHTML = '✔ Password Validated';
    }
}
function emva() {
    var emval = document.getElementById('email').value;
    if ((emval.length > 50)) {
        document.getElementById('emailspans').innerHTML = '';
        document.getElementById('emailspan').innerHTML = '❌ Email should be of less than 50 digits';
    }
    if ((emval.length <= 50)) {
        document.getElementById('emailspan').innerHTML = '';
        document.getElementById('emailspans').innerHTML = '✔ Email Validated';
    }
    if (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in') <= 0) {
        document.getElementById('emailspans').innerHTML = '';
        document.getElementById('emailspan').innerHTML = '❌ Invalid Email';
    }
    if (emval.indexOf('@gmail.com') > 0 || emval.indexOf('@srmist.edu.in') > 0) {
        document.getElementById('emailspan').innerHTML = '';
        document.getElementById('emailspans').innerHTML = '✔ Email Validated';
    }
}
function unva() {
    var unval = document.getElementById('username').value;
    if (!isNaN(unval)) {
        document.getElementById('userspans').innerHTML = '';
        document.getElementById('userspan').innerHTML = '❌ Only Characters are allowed';
    }
    if ((unval.length > 50)) {
        document.getElementById('userspans').innerHTML = '';
        document.getElementById('userspan').innerHTML = '❌ Username should be of less than 50 digits';
    }
    if ((unval.length <= 50) && (isNaN(unval))) {
        document.getElementById('userspan').innerHTML = '';
        document.getElementById('userspans').innerHTML = '✔ Username Validated';
    }
}
function uava() {
    var uaval = document.getElementById('useradd').value;
    if ((uaval.length > 100)) {
        document.getElementById('useraddspans').innerHTML = '';
        document.getElementById('useraddspan').innerHTML = '❌ Address should be of less than 100 digits';
    }
    if ((uaval.length <= 100)) {
        document.getElementById('useraddspan').innerHTML = '';
        document.getElementById('useraddspans').innerHTML = '✔ Address Validated';
    }
    if ((uaval.length == '')) {
        document.getElementById('useraddspans').innerHTML = '';
        document.getElementById('useraddspan').innerHTML = '❌ Address cannot be blank';
    }
}

function labnumva(){
  var labnum = document.getElementById('labnum').value; 
  if ((labnum.length == 0 || labnum == "") || labnum.length > 8) {
    if(labnum.length == 0 || labnum == ""){
      document.getElementById('mospans').innerHTML = '';
      document.getElementById('mospan').innerHTML = '❌ Lab Number cannot be Blank';
    }
    if(labnum.length > 8){
      document.getElementById('mospans').innerHTML = '';
      document.getElementById('mospan').innerHTML = '❌ Lab Number should be of max 8 digits';
    }
    return false;
  }
  else {
    document.getElementById('mospan').innerHTML = '';
    document.getElementById('mospans').innerHTML = '✔ Lab Number Validated';
  }
  return true;
}
function regva(){ 
  var userreg = document.getElementById('userreg').value; 
  if(userreg != ""){
    document.getElementById('userregspan').innerHTML = '';
    document.getElementById('userregspans').innerHTML = '✔ Reg Validated';
  } 
}
function labnameva(){
  var labname = document.getElementById('labname').value; 
  var regex = /[^ a-z 1234567890]/gi;
  document.getElementById('labname').value = labname.replace(regex, ""); 
  if(labname == ""){
    document.getElementById('rfidspans').innerHTML = '';
    document.getElementById('rfidspan').innerHTML = '❌ Lab Name cannot be Blank';
    return false;
  }
  else{
    document.getElementById('rfidspan').innerHTML = '';
    document.getElementById('rfidspans').innerHTML = '✔ Lab Name Validated';
  }
  return true;
}

function mova() {
  var moval = document.getElementById('mono').value; 
  if (moval.length >13 || moval.length == 0 || moval.length < 10) {
    document.getElementById('mospans').innerHTML = '';
    document.getElementById('mospan').innerHTML = '❌ Invalid Number';
  }
  else if(moval.length == 10){
    document.getElementById('mospans').innerHTML = '';
    document.getElementById('mospan').innerHTML = '❌ Country Code';
  }
  else {
    document.getElementById('mospan').innerHTML = '';
    document.getElementById('mospans').innerHTML = '✔ Phone Validated';
  } 
}
function rfva() {
  var rfid = document.getElementById('rfid').value; 
    if (rfid.length == 0) {
      document.getElementById('rfidspans').innerHTML = '';
      document.getElementById('rfidspan').innerHTML = '❌ RFID cannot be Blank';
    }
    else {
      document.getElementById('rfidspan').innerHTML = '';
      document.getElementById('rfidspans').innerHTML = '✔ RFID Validated';
    } 
}
function approve(){
  var moval = document.getElementById('mono').value; 
      if (moval.length >13 || moval.length == 0 || moval.length <= 10) {
        document.getElementById('mospans').innerHTML = '';
        document.getElementById('mospan').innerHTML = '❌ Invalid Number';
        return false;
      }
      else {
        document.getElementById('mospan').innerHTML = '';
        document.getElementById('mospans').innerHTML = '✔ Phone Validated';
      }
  var rfid = document.getElementById('rfid').value; 
    if (rfid.length == 0) {
      document.getElementById('rfidspans').innerHTML = '';
      document.getElementById('rfidspan').innerHTML = '❌ RFID cannot be Blank';
      return false;
    }
    else {
      document.getElementById('rfidspan').innerHTML = '';
      document.getElementById('rfidspans').innerHTML = '✔ RFID Validated';
    } 
    return true;
}
function ova() {
    var oval = document.getElementById('ono').value;
    if (oval.length != 8) {
        document.getElementById('ospans').innerHTML = '';
        document.getElementById('ospan').innerHTML = '❌ OTP should be of 8 digits';
    }
    else {
        document.getElementById('ospan').innerHTML = '';
        document.getElementById('ospans').innerHTML = '✔ OTP Validated';
    }
}
function otpva() {
    var oval = document.getElementById('ono').value;
    if (oval.length != 8) {
        document.getElementById('otperr').innerHTML = '❌ OTP should be of 8 digits';
        return false;
    }
    else{
      return true;
    }
}
function subva() {
    var unval = document.getElementById('username').value;
    var uaval = document.getElementById('useradd').value;
    
    var moval = document.getElementById('mono').value;
    var emval = document.getElementById('email').value;
    var psdval = document.getElementById('password').value;
    var cpsdval = document.getElementById('cpassword').value;


    if ((unval == '') || (uaval == '') || (moval == '') || (emval == '') || (psdval == '') || (cpsdval == '') || (psdval != cpsdval) || (psdval.length > 25) || (emval.length > 50) || (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in') <= 0) || (!isNaN(unval)) || (unval.length > 50) || (moval.length >13 || moval.length <=10)) {

        if (!isNaN(unval)) {
            document.getElementById('userspans').innerHTML = '';
            document.getElementById('userspan').innerHTML = '❌ Only Characters are allowed';
        }
        if (unval == '') {
            document.getElementById('userspans').innerHTML = '';
            document.getElementById('userspan').innerHTML = '❌ Username cannot be blank';
        }
        if (uaval == '') {
            document.getElementById('useraddspans').innerHTML = '';
            document.getElementById('useraddspan').innerHTML = '❌ Address cannot be blank';
        }
        if (psdval == '') {
            document.getElementById('psdspans').innerHTML = '';
            document.getElementById('psdspan').innerHTML = '❌ Password cannot be blank';
        }
        if (cpsdval == '') {
            document.getElementById('cpsdspans').innerHTML = '';
            document.getElementById('cpsdspan').innerHTML = '❌ Confirm Password cannot be blank';
        }
        if ((psdval != cpsdval)) {
            document.getElementById('cpsdspans').innerHTML = '';
            document.getElementById('cpsdspan').innerHTML = '❌ Passwords do not match';
        }
        if ((psdval.length > 25)) {
            document.getElementById('psdspans').innerHTML = '';
            document.getElementById('psdspan').innerHTML = '❌ Password should be of less than 25 digits';
        }
        if ((emval.length > 50)) {
            document.getElementById('emailspans').innerHTML = '';
            document.getElementById('emailspan').innerHTML = '❌ Email should be of less than 50 digits';
        }
        if (emval.indexOf('@gmail.com') <= 0 && emval.indexOf('@srmist.edu.in') <= 0) {
            document.getElementById('emailspans').innerHTML = '';
            document.getElementById('emailspan').innerHTML = '❌ Invalid Email';
        }

        if (moval.length >13 || moval.length <=10) {
            document.getElementById('mospans').innerHTML = '';
            document.getElementById('mospan').innerHTML = '❌ Invalid Number';
        }
        if (moval == '') {
            document.getElementById('mospans').innerHTML = '';
            document.getElementById('mospan').innerHTML = '❌ Number cannot be blank';
        }
        if (emval == '') {
            document.getElementById('emailspans').innerHTML = '';
            document.getElementById('emailspan').innerHTML = '❌ Email cannot be blank';
        }
        if ((unval.length > 50)) {
            document.getElementById('userspans').innerHTML = '';
            document.getElementById('userspan').innerHTML = '❌ Username should be of less than 50 digits';
        }
        if ((uaval.length > 100)) {
            document.getElementById('useraddspans').innerHTML = '';
            document.getElementById('useraddspan').innerHTML = '❌ Address should be of less than 100 digits';
        }
        document.getElementById('suberr').innerHTML = '❌ Invalid Data Submitted !!!';
        document.getElementById('suberr').style.background = "white";
        return false;
    }
    else{
      return true;
    }
}
// function payva() {
// var username = document.getElementById('username').value; 
// var pa = document.getElementById('pa').value;
// var pi = document.getElementById('pi').value; 

// var pdate = document.getElementById('pdate').value; 



// if ((username == '') || (pdate == '') || (pa == '') || (pi == '') ) {

     
//     if (username == '') {
//         document.getElementById('userspans').innerHTML = '';
//         document.getElementById('userspan').innerHTML = '❌ Username cannot be blank';
//     }
//     if (pdate == '') {
//         document.getElementById('pdspans').innerHTML = '';
//         document.getElementById('pdspan').innerHTML = '❌ Date cannot be blank';
//     }
//     if (pa == '') {
//         document.getElementById('paspans').innerHTML = '';
//         document.getElementById('paspan').innerHTML = '❌ Payed Amount cannot be blank';
//     }
//     if (pi == '') {
//         document.getElementById('pispans').innerHTML = '';
//         document.getElementById('pispan').innerHTML = '❌ Payed Interest cannot be blank';
//     }  

//     document.getElementById('suberr').innerHTML = '❌ Invalid Data Submitted !!!';
//     document.getElementById('suberr').style.background = "white";
//     return false;
// }
// }

// function appva() {
//     var username = document.getElementById('username').value;
//     var todate = document.getElementById('todate').value;
//     var la = document.getElementById('la').value; 


//     if ((username == '') || (todate == '') || (la == '')) {

        
//         if (username == '') {
//             document.getElementById('userspans').innerHTML = '';
//             document.getElementById('userspan').innerHTML = '❌ Username cannot be blank';
//         }
//         if (todate == '') {
//             document.getElementById('tdspans').innerHTML = '';
//             document.getElementById('tdspan').innerHTML = '❌ Date cannot be blank';
//         }
//         if (la == '') {
//             document.getElementById('laspans').innerHTML = '';
//             document.getElementById('laspan').innerHTML = '❌ Loan Amount cannot be blank';
//         }  
 
//         document.getElementById('suberr').innerHTML = '❌ Invalid Data Submitted !!!';
//         document.getElementById('suberr').style.background = "white";
//         return false;
//     }
// }