function myFunction() {

  }

var darkmodebutton = document.querySelectorAll(".darkmodebutton")


document.addEventListener('DOMContentLoaded', function() {

    for(let i = 0; i < darkmodebutton.length; i++) {

        darkmodebutton.addEventListener('click', function(event) {

            alert("Hello!")
            // var element = document.body;
            // element.classList.toggle("dark-mode");

        })

    }


})
