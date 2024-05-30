// JavaScript source code

//main function

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_original_width;

var canvas_original_height;

//WebGL related variables

const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_TEXTURE: 1
};

var shaderProgramObject = null;
var vao = null;
var vbo = null;
var vbo_TexCoord = null;

var textureSamplerUniform = 0;

var mvpMatrixUniform;

var perspectiveProjectionMatrix;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

//checkerboard related variables

const CHECK_IMAGE_HEIGHT = 64; 
const CHECK_IMAGE_WIDTH = 64;  

var C_Angle = 0.0;

//texture variables

var TEXTURE_CHECKER_BOARD = 0;

var checkImage = new Uint8Array(CHECK_IMAGE_HEIGHT * CHECK_IMAGE_WIDTH * 4);
                 

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

    //set canvas width and height for future use

    canvas_original_width = canvas.width;

    canvas_original_height = canvas.height;

    //register for keyboard events

    window.addEventListener("keydown", keyDown, false);

    //register for muose events

    window.addEventListener("click", mouseDown, false);

    //register for resize events

    window.addEventListener("resize", resize, false);

    initialize();

    resize();

    display();

}

function keyDown(event)
{
    //code

    switch (event.keyCode)
    {
        case 81:
        case 113:
            uninitialize();
            window.close();   //not browser neutral

        case 70:
        case 102:
            toggleFullscreen();
            
            break;
    }
}

function mouseDown(event)
{
    //alert("Mouse is clicked");
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

        bFullScreen = true;
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

        bFullScreen = false;
    }


}

function initialize()
{
    //code

    //get context from above canvas

    gl = canvas.getContext("webgl2");

    if (!gl)
    {
        console.log("Did not get WebGL2 context Successfully \n");

    }

    else
    {
        console.log("Got WebGL2 context successfully \n");
    }

    //set webgl2 context's view width and view height properties

    gl.viewportWidth = canvas.width;

    gl.viewportHeight = canvas.height;

    //vertex shader

    var vertexShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "in vec4 aPosition;" +
            "in vec2 aTexCoord;" +
            "out vec2 oTexCoord;" +
            "uniform mat4 uMVPMatrix;" +
            "void main(void)" +
            "{" +
            "gl_Position=uMVPMatrix*aPosition;" +
            "oTexCoord = aTexCoord;" +
            "}"
        );

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);

        if (error.length > 0)
        {
            var log = "vertex shader compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("Vertex Shader Compiled Successfully !\n");

    }

    var fragmentShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "out vec4 FragColor;" +
            "in vec2 oTexCoord;" +
            "uniform highp sampler2D uTextureSampler;" +
            "void main(void)" +
            "{" +
            "FragColor = texture(uTextureSampler, oTexCoord);" +
            "}"
        );

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(fragmentShaderObject);

        if (error.length > 0)
        {
            var log = "fragment shader compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("fragment Shader Compiled Successfully !\n");

    }

    //shader program

    shaderProgramObject = gl.createProgram();

    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_TEXTURE, "aTexCoord");

    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject);

        if (error.length > 0) {
            var log = "shader program linking error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("shader program linked Successfully !\n");
    }

    
    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");

    textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "uTextureSampler");

    //geometry position array declaration

   /* var pyramidPosition = new Float32Array
    ([
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0,-1.0,0.0
    ])*/

    var rectangleTexCoord = new Float32Array([
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0
    ]);

    var sizeInBytes = 4 * 3 * 4;

    //vao

    vao = gl.createVertexArray();

    gl.bindVertexArray(vao);

    //vbo

    vbo = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);

    gl.bufferData(gl.ARRAY_BUFFER, sizeInBytes, gl.DYNAMIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //vbo for texture coordinates

    vbo_TexCoord = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_TexCoord);

    gl.bufferData(gl.ARRAY_BUFFER, rectangleTexCoord, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_TEXTURE, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_TEXTURE);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo


    //unbind vao

    gl.bindVertexArray(null);
        

    //depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);
    

    // set clear color

    gl.clearColor(0.6, 0.6, 0.6, 1.0);

    loadGLTexture();

    //initialize projection matrix

    perspectiveProjectionMatrix = mat4.create();

}

function resize()
{
    //code

    if (bFullScreen == true)
    {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }

    else
    {

        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    // set viewport

    gl.viewport(0, 0, canvas.width, canvas.height);

    //set perspective projection

    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);
    
}



function display()
{
    //code

    var square1Position = new Float32Array(18);
    var square2Position = new Float32Array(18);

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    //transformation

    var modelViewMatrix = mat4.create();

    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -4.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    //for first quad

    square1Position[0] = 0.0;
    square1Position[1] = 1.0;
    square1Position[2] = 0.0;

    square1Position[3] = -2.0;
    square1Position[4] = 1.0;
    square1Position[5] = 0.0;

    square1Position[6] = -2.0;
    square1Position[7] = -1.0;
    square1Position[8] = 0.0;

    square1Position[9] = -2.0;
    square1Position[10] = -1.0;
    square1Position[11] = 0.0;


    square1Position[12] = 0.0;
    square1Position[13] = -1.0;
    square1Position[14] = 0.0;

    square1Position[15] = 0.0;
    square1Position[16] = 1.0;
    square1Position[17] = 0.0;



    gl.activeTexture(gl.TEXTURE0);

    gl.bindTexture(gl.TEXTURE_2D, TEXTURE_CHECKER_BOARD);

    gl.uniform1i(textureSamplerUniform, 0);

    gl.bindVertexArray(vao);

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
    gl.bufferData(gl.ARRAY_BUFFER, square1Position, gl.DYNAMIC_DRAW);

    


    gl.drawArrays(gl.TRIANGLES, 0, 6);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    //second quad

    //transformation

    modelViewMatrix = mat4.create();

    modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -4.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    //for second quad (tilted)

    square2Position[0] = 2.41421;
    square2Position[1] = 1.0;
    square2Position[2] = -1.41421;

    square2Position[3] = 1.0;
    square2Position[4] = 1.0;
    square2Position[5] = 0.0;

    square2Position[6] = 1.0;
    square2Position[7] = -1.0;
    square2Position[8] = 0.0;

    square2Position[9] = 1.0;
    square2Position[10] = -1.0;
    square2Position[11] = 0.0;

    square2Position[12] = 2.41421;
    square2Position[13] = -1.0;
    square2Position[14] = -1.41421;

    square2Position[15] = 2.41421;
    square2Position[16] = 1.0;
    square2Position[17] = -1.41421;

    gl.activeTexture(gl.TEXTURE0);

    gl.bindTexture(gl.TEXTURE_2D, TEXTURE_CHECKER_BOARD);

    gl.uniform1i(textureSamplerUniform, 0);

    gl.bindVertexArray(vao);

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);

    gl.bufferData(gl.ARRAY_BUFFER, square2Position, gl.DYNAMIC_DRAW);




    gl.drawArrays(gl.TRIANGLES, 0, 6);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);


    gl.bindVertexArray(null);

    gl.bindTexture(gl.TEXTURE_2D, null);

    gl.useProgram(null);

    update();

    //double buffering

    requestAnimationFrame(display, canvas);
    
}

function update()
{

    //code
}

/*function loadGLTexture() {
    smiley_Texture = gl.createTexture();

    smiley_Texture.image = new Image();

    smiley_Texture.image.src = "smiley.png";

    smiley_Texture.image.onload = function () {
        gl.bindTexture(gl.TEXTURE_2D, smiley_Texture);

        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, smiley_Texture.image);

        gl.generateMipmap(gl.TEXTURE_2D);

        gl.bindTexture(gl.TEXTURE_2D, null);
    }

}*/

// Define loadGLTexture function
function loadGLTexture()
{
    // Call makeCheckImage to generate the image data
    makeCheckImage();

    // Step 7: Generate texture
    TEXTURE_CHECKER_BOARD = gl.createTexture();

    gl.bindTexture(gl.TEXTURE_2D, TEXTURE_CHECKER_BOARD);

    

    // Step 9: Set pixel storage mode (UNPACK_ALIGNMENT)
    gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);

    // Step 10: Set texture parameters
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

    // Step 11: Upload the texture image data
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, CHECK_IMAGE_WIDTH, CHECK_IMAGE_HEIGHT, 0, gl.RGBA, gl.UNSIGNED_BYTE, checkImage);

    // Step 12: Unbind the texture
    gl.bindTexture(gl.TEXTURE_2D, null);

    // Step 13: Any additional steps can be added here
    // ...
}


function makeCheckImage()
{
    // Loop through each pixel
    for (let i = 0; i < CHECK_IMAGE_HEIGHT; i++)
    {
        for (let j = 0; j < CHECK_IMAGE_WIDTH; j++)
        {
            // Calculate checkerboard pattern
            let c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;

            // Calculate the index for the flat array
            let index = (i * CHECK_IMAGE_WIDTH + j) * 4;

            // Set the RGBA values
            checkImage[index] = c;     // Red
            checkImage[index + 1] = c; // Green
            checkImage[index + 2] = c; // Blue
            checkImage[index + 3] = 255; // Alpha
        }
    }
}

function uninitialize()
{

    if (shaderProgramObject)  //this statement can be shaderProgramObject != null
    {
        gl.useProgram(shaderProgramObject);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject);

        if (shaderObjects && shaderObjects.length > 0)
        {
            for (let i = 0; i < shaderObjects.length; i++)
            {
                gl.detachShader(shaderProgramObject, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;

            }


        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject);
        shaderProgramObject = null;


    }

    if (vbo)
    {
        gl.deleteBuffer(vbo);
        vbo = null;
    }

    if (vao)
    {
        gl.deleteVertexArray(vao);
        vao = null;
    }



    

    
}





