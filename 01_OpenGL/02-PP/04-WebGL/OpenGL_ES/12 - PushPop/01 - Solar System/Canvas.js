// JavaScript source code

//main function

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_original_width;

var canvas_original_height;

//matrix stack size

var MODEL_VIEW_MATRIX_STACK = 32;


//WebGL related variables

const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_NORMAL: 1,
    AMC_ATTRIBUTE_TEXTURE: 2
};

var shaderProgramObject = null;

var mvpMatrixUniform;

var colorUniform = 0;

var perspectiveProjectionMatrix;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

//sphere related variable

var sphere = null;

//PushPop Related variables

/*mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
int matrixStackTop = -1;*/

let matrixStack = [];

var matrixStackTop = -1;

//fill array with 32 identity matrices



//rotation related variables

var day = 0.0;
var year = 0.0;

var moon_day = 0.0;
var moon_year = 0.0;

//int revolve earth clock wise




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

    console.log(event);

    switch (event.keyCode)
    {
        case 81:
        case 113:
            uninitialize();
            window.close();   //not browser neutral
            break;

        case 70:
        case 102:
            toggleFullscreen();
            
            break;

       



            

            
    }

    switch (event.key)
    {
        case 'y':

            year += 0.05;

            if (year > 360.0)
            {
                year = -360.0;
            }

            break;

        case 'Y':

            year -= 0.05;

            if (year < -360.0)
            {
                year = 0.0;
            }

            break;

        case 'D':

            day += 0.05;

            if (day > 360.0)
            {
                day = -360.0;
            }

            break;

        case 'd':

            day -= 0.05;

            if (day < -360.0)
            {
                day = 0.0;
            }

            break;

        case 'm':

            moon_year += 0.05;

            if (moon_year > 360.0)
            {
                moon_year = -360.0;
            }

            break;

        case 'M':

            moon_year -= 0.05;

            if (moon_year < -360.0)
            {
                moon_year = 0.0;
            }

            break;

        case 'N':

            moon_day += 0.05;

            if (moon_day > 360.0)
            {
                moon_day = -360.0;
            }

            break;

        case 'n':

            moon_day -= 0.05;

            if (moon_day < -360.0)
            {
                moon_day = 0.0;
            }

            break;
    }

    console.log(year);
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
            "uniform mat4 uMVPMatrix;" +
            "void main(void)" +
            "{" +
            "gl_Position=uMVPMatrix*aPosition;" +
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
            "uniform highp vec4 colorUniform;"+
            "void main(void)" +
            "{" +
            "FragColor = colorUniform;" +
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

    colorUniform = gl.getUniformLocation(shaderProgramObject, "colorUniform");

    //geometry position array declaration

    /*var pyramidPosition = new Float32Array
    ([
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0,-1.0,0.0
    ])*/

    sphere = new Mesh();
    makeSphere(sphere, 2.0, 40, 40);


    /*//vao

    vao = gl.createVertexArray();

    gl.bindVertexArray(vao);

    //vbo

    vbo = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);

    gl.bufferData(gl.ARRAY_BUFFER, pyramidPosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);*/
        

    //depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);
    

    // set clear color

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

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

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    //transformation for the sun

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelViewProjectionMatrix = mat4.create();




    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -9.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

    //gl.polygonMode(gl.FRONT_AND_BACK, gl.LINE);



    pushMatrix(modelMatrix);
    {
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, viewMatrix);
        mat4.multiply(modelViewProjectionMatrix, modelViewProjectionMatrix, modelMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

        gl.uniform4f(colorUniform, 1.0, 0.5, 0.0, 1.0);

        sphere.draw();

        //earth related transformations

        var eyRotationMatrix = mat4.create();

        var edRotationMatrix = mat4.create();

        translationMatrix = mat4.create();

        //initialize Translation matrix

        mat4.translate(translationMatrix, translationMatrix, [4.7, 0.0, 0.0]);

        //initialize rotation matrix

        mat4.rotate(eyRotationMatrix, eyRotationMatrix, year, [0.0, 1.0, 0.0]);

        mat4.rotate(edRotationMatrix, edRotationMatrix, day, [0.0, 1.0, 0.0]);

        //mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        mat4.multiply(modelMatrix, modelMatrix, eyRotationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, edRotationMatrix);

        pushMatrix(modelMatrix);
        {
            var scaleMatrix = mat4.create();

            //mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -9.0]);

            mat4.scale(scaleMatrix, scaleMatrix, [0.5, 0.5, 0.5]);

            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, viewMatrix);
            mat4.multiply(modelViewProjectionMatrix, modelViewProjectionMatrix, modelMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            gl.uniform4f(colorUniform, 0.4, 0.9, 1.0, 1.0);

            sphere.draw();
        }
        modelMatrix = popMatrix();

        //moon related transformations

        mat4.translate(translationMatrix, translationMatrix, [-0.05, 0.0, 0.0]);

        //initialize rotation matrix

        var myRotationMatrix = mat4.create();

        var mdRotationMatrix = mat4.create();

        mat4.rotate(myRotationMatrix, myRotationMatrix,moon_year, [0.0, 1.0, 0.0]);

        mat4.rotate(mdRotationMatrix, mdRotationMatrix, moon_day, [0.0, 1.0, 0.0]);

        mat4.multiply(modelMatrix, modelMatrix, myRotationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, mdRotationMatrix);

        pushMatrix(modelMatrix);
        {
            var scaleMatrix = mat4.create();

            mat4.scale(scaleMatrix, scaleMatrix, [0.3, 0.3, 0.3]);

            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, viewMatrix);
            mat4.multiply(modelViewProjectionMatrix, modelViewProjectionMatrix, modelMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            gl.uniform4f(colorUniform, 1.0, 1.0, 1.0, 1.0);

            sphere.draw();


        }
        modelMatrix = popMatrix();
        


    }
    modelMatrix = popMatrix();
    
    
    
   



   /* //transformation

    var modelViewMatrix = mat4.create();

    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -5.3]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    gl.uniform4f(colorUniform, 1.0, 0.5, 0.0, 1.0);

    sphere.draw();*/

    

   /* gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLES, 0, 3);

    gl.bindVertexArray(null);
*/

    

    gl.useProgram(null);

    update();

    //double buffering

    requestAnimationFrame(display, canvas);
    
}

function update()
{

    /*if (revolveEarthC == 1)
    {
        if (year < 360.0)
        {
            year += 0.05;
        }
        else
        {
            year = 0.0
        }
    }

    if (revolveEarthC == 0)
    {
        if (year > -360.0)
        {
            year -= 0.05;
        }
        else
        {
            year = 0.0
        }
    }*/

    //code
}

function initializeMatrixStack()
{
    matrixStackTop = 0;

    for (let i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
    {
        matrixStack.push(mat4.create());
    }
}

function pushMatrix(matrix)
{
    if (matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
    {
        console.error("ERROR: Exceeded matrix stack limit");
        uninitialize();
    }

    matrixStack[matrixStackTop] = matrix;

    matrixStackTop++;
}

function popMatrix()
{
    if (matrixStackTop < 0)
    {
        console.error("ERROR: Matrix Stack is empty");

        uninitialize();
    }

    matrixStack[matrixStackTop] = mat4.create();

    matrixStackTop--;

    var matrix = mat4.create();

	matrix = matrixStack[matrixStackTop];

    return matrix;
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

    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }




    

    
}





