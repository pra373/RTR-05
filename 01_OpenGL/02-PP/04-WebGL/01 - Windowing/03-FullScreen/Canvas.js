// JavaScript source code

//main function

var canvas = null;

var context = null;

function main()
{
    //get canvas
    canvas = document.getElementById("PLP");

    if (!canvas)
    {
        console.log("Did not get canvas \n");

    }
    else
    {
        console.log("Got canvas successfully \n");
    }

    //get context from above canvas

    context = canvas.getContext("2d");

    if (!context)
    {
        console.log("Did not get context Successfully \n");

    }

    else
    {
        console.log("Got context successfully \n");
    }

    //set canvas background color to black

    context.fillStyle = "black";

    //paint the canvas rectangle background with above color

    context.fillRect(0, 0, canvas.width, canvas.height);

    //draw Text

    drawText("Hello World !!!");

    //register for keyboard events

    window.addEventListener("keydown", keyDown, false);

    //register for muose events

    window.addEventListener("click", mouseDown, false);



}

function keyDown(event)
{
    //code

    switch (event.keyCode)
    {
        case 70:
        case 102:
            toggleFullscreen();
            drawText("Hello World !!!");
            break;
    }
}

function mouseDown(event)
{
    alert("Mouse is clicked");
} 

function drawText(text)
{
    //code

    context.textAlign = "center";  //horizontal centre

    context.textBaseLine = "middle"; //vertical centre

    //set Font

    context.font = "48px sans-serif";

    //set text color to white

    context.fillStyle = "white";

    //paint the text with above color

    context.fillText(text, (canvas.width / 2), (canvas.height / 2));

}

function toggleFullscreen()
{
    var fullscreen_element =
        document.fullscreenElement ||
        document.webkitFullscreenElement ||
        document.mozFullScreenElement ||
        document.msFullscreenElement ||
        null; 

    //if not full Screen

    if (fullscreen_element == null)
    {
        if (canvas.requestFullscreen)
        {
            canvas.requestFullscreen();
        }

        else if (canvas.webkitRequestFullscreen)
        {
            canvas.webkitRequestFullscreen();
        }

        else if (canvas.mozRequestFullScreen)
        {
            canvas.mozRequestFullScreen();
        }

        else if (canvas.msRequestFullscreen)
        {
            canvas.msRequestFullscreen();
        }
    }
    else    //if already full screen
    {
        if (document.exitFullscreen)
        {
            document.exitFullscreen();
        }

        else if (document.webkitExitFullscreen)
        {
            document.webkitExitFullscreen();
        }

        else if (document.mozCancelFullScreen)
        {
            document.mozCancelFullScreen();
        }

        else if (document.msExitFullscreen)
        {
            document.msExitFullscreen();
        }
    }


}

