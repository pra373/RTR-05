// JavaScript source code

//main function

function main()
{
    //get canvas
    var canvas = document.getElementById("PLP");

    if (!canvas)
    {
        console.log("Did not get canvas \n");

    }
    else
    {
        console.log("Got canvas successfully \n");
    }

    //get context from above canvas

    var context = canvas.getContext("2d");

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

    //declare the text
    var text = "Hello World !!!";

    

    
    //set the text in middle of the canvas

    context.textAlign = "center";  //horizontal centre

    context.textBaseLine = "middle"; //vertical centre

    //set Font

    context.font = "48px sans-serif";

    //set text color to white

    context.fillStyle = "white";

    //paint the text with above color

    context.fillText(text, (canvas.width / 2), (canvas.height / 2));

    //register for keyboard events

    window.addEventListener("keydown", keyDown, false);

    //register for muose events

    window.addEventListener("click", mouseDown, false);



}

function keyDown(event)
{
    alert("Key is Pressed");
}

function mouseDown(event)
{
    alert("Mouse is clicked");
}
 

