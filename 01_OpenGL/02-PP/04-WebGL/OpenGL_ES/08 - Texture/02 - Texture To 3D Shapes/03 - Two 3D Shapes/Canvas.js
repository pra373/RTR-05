// JavaScript source code

//main function

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_original_width;

var canvas_original_height;

//texture variable

var kundali_Texture = 0;

var stone_Texture = 0;

//texture sampler uniform

var textureSamplerUniform = 0;

//rotation variable

var angleC = 0.0;

var angleP = 0.0;


//WebGL related variables

const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_TEXTURE: 1
};

var shaderProgramObject = null;

var vao_Cube = null;
var vao_Pyramid = null;

var vbo_Position_Cube = null;
var vbo_Position_Pyramid = null;



//textrure vbo

var vbo_TexCoord_Cube;

var vbo_TexCoord_Pyramid;

var mvpMatrixUniform;

var perspectiveProjectionMatrix;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;
                 

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
            "oTexCoord = aTexCoord;"+
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

    var cubePosition = new Float32Array
    ([
        // top
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,

        1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,

        // bottom
        1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0,

        1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // front
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,

        1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // back
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,

        1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,

        // right
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, -1.0, 1.0,

        1.0, 1.0, -1.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,

        // left
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,

        -1.0, 1.0, 1.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0
    ])

    var cubeTexCoord = new Float32Array
        ([
            // top
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,

            // bottom
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,

            // front
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,

            // back
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,

            // right
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,

            // left
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0
           
        ])


    var pyramidPosition = new Float32Array
        ([
            // front
            0.0, 1.0, 0.0, // front-top
            -1.0, -1.0, 1.0, // front-left
            1.0, -1.0, 1.0, // front-right

            // right
            0.0, 1.0, 0.0, // right-top
            1.0, -1.0, 1.0, // right-left
            1.0, -1.0, -1.0, // right-right

            // back
            0.0, 1.0, 0.0, // back-top
            1.0, -1.0, -1.0, // back-left
            -1.0, -1.0, -1.0, // back-right

            // left
            0.0, 1.0, 0.0, // left-top
            -1.0, -1.0, -1.0, // left-left
            -1.0, -1.0, 1.0 // left-right
        ])

    var pyramidTexCoord = new Float32Array
        ([
            // front
            0.5, 1.0, // front-top
            0.0, 0.0, // front-left
            1.0, 0.0, // front-right

            // right
            0.5, 1.0, // right-top
            1.0, 0.0, // right-left
            0.0, 0.0, // right-right

            // back
            0.5, 1.0, // back-top
            0.0, 0.0, // back-left
            1.0, 0.0, // back-right

            // left
            0.5, 1.0, // left-top
            1.0, 0.0, // left-left
            0.0, 0.0, // left-right

        ])


    //vao

    vao_Cube = gl.createVertexArray();

    gl.bindVertexArray(vao_Cube);

    //vbo

    vbo_Position_Cube = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Position_Cube);

    gl.bufferData(gl.ARRAY_BUFFER, cubePosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo



    //vbo for texture coordinates

    vbo_TexCoord_Cube = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_TexCoord_Cube);

    gl.bufferData(gl.ARRAY_BUFFER, cubeTexCoord, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_TEXTURE, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_TEXTURE);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo



    //unbind vao

    gl.bindVertexArray(null);

    //vao

    vao_Pyramid = gl.createVertexArray();

    gl.bindVertexArray(vao_Pyramid);

    //vbo

    vbo_Position_Pyramid = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Position_Pyramid);

    gl.bufferData(gl.ARRAY_BUFFER, pyramidPosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo



    //vbo for texture coordinates

    vbo_TexCoord_Pyramid = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_TexCoord_Pyramid);

    gl.bufferData(gl.ARRAY_BUFFER, pyramidTexCoord, gl.STATIC_DRAW);

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

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    //image loading and texture defining function

    loadGLTexture1();
    loadGLTexture2();

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

    //transformation for cube

    var modelViewMatrix = mat4.create();

    var rotationMatrix1 = mat4.create();

    var rotationMatrix2 = mat4.create();

    var rotationMatrix3 = mat4.create();

    var translationMatrix = mat4.create();

    var modelViewProjectionMatrix = mat4.create();

    // Set rotation angles

    mat4.rotate(rotationMatrix1, mat4.create(), angleC, [1, 0, 0]);
    mat4.rotate(rotationMatrix2, mat4.create(), angleC, [0, 1, 0]);
    mat4.rotate(rotationMatrix3, mat4.create(), angleC, [0, 0, 1]);



    mat4.translate(translationMatrix, mat4.create(), [1.7, 0.0, -7.5]);

    // Combine rotations with the modelViewMatrix

    mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
    mat4.multiply(modelViewMatrix, modelViewMatrix, rotationMatrix1);
    mat4.multiply(modelViewMatrix, modelViewMatrix, rotationMatrix2);
    mat4.multiply(modelViewMatrix, modelViewMatrix, rotationMatrix3);


    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);


    gl.activeTexture(gl.TEXTURE0);

    gl.bindTexture(gl.TEXTURE_2D, kundali_Texture);

    gl.uniform1i(textureSamplerUniform, 0);

    gl.bindVertexArray(vao_Cube);

    gl.drawArrays(gl.TRIANGLES, 0, 36);

    gl.bindVertexArray(null);

    gl.bindTexture(gl.TEXTURE_2D, null);

    //transformation for Pyramid

    //transformation

    modelViewMatrix = mat4.create();

    modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [-1.8, 0.0, -5.7]);

    mat4.rotate(modelViewMatrix, modelViewMatrix, angleP, [0, 1, 0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    gl.activeTexture(gl.TEXTURE0);

    gl.bindTexture(gl.TEXTURE_2D, stone_Texture);

    gl.uniform1i(textureSamplerUniform, 0);

    gl.bindVertexArray(vao_Pyramid);

    gl.drawArrays(gl.TRIANGLES, 0, 12);

    gl.bindVertexArray(null);

    gl.bindTexture(gl.TEXTURE_2D, null);


    gl.useProgram(null);

   

    update();

    //double buffering

    requestAnimationFrame(display, canvas);
    
}

function update()
{

    //Triangle Rotation

    if (angleC < 360.0)
    {
        angleC = angleC + 0.007;
    }

    else
    {
        angleC = -360.0;
    }

    //pyramid rotation

    if (angleP < 360.0)
    {
        angleP = angleP + 0.007;
    }

    else
    {
        angleP = -360.0;
    }
    
}

function loadGLTexture1()
{
    kundali_Texture = gl.createTexture();

    kundali_Texture.image = new Image();

    kundali_Texture.image.src = "Vijay_Kundali.png";

    kundali_Texture.image.onload = function ()
    {
        gl.bindTexture(gl.TEXTURE_2D, kundali_Texture);

        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, kundali_Texture.image);

        gl.generateMipmap(gl.TEXTURE_2D);

        gl.bindTexture(gl.TEXTURE_2D, null);
    }

}

function loadGLTexture2() {
    stone_Texture = gl.createTexture();

    stone_Texture.image = new Image();

    stone_Texture.image.src = "stone.png";

    stone_Texture.image.onload = function () {
        gl.bindTexture(gl.TEXTURE_2D, stone_Texture);

        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, stone_Texture.image);

        gl.generateMipmap(gl.TEXTURE_2D);

        gl.bindTexture(gl.TEXTURE_2D, null);
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

    if (vbo_TexCoord_Cube)
    {
        gl.deleteBuffer(vbo_TexCoord_Cube);
        vbo_TexCoord_Cube = null;
    }

    if (vbo_Position_Cube)
    {
        gl.deleteBuffer(vbo_Position_Cube);
        vbo_Position_Cube = null;
    }

    if (vao_Cube)
    {
        gl.deleteVertexArray(vao_Cube);
        vao_Cube = null;
    }

    if (vbo_TexCoord_Pyramid) {
        gl.deleteBuffer(vbo_TexCoord_Pyramid);
        vbo_TexCoord_Pyramid = null;
    }

    if (vbo_Position_Pyramid) {
        gl.deleteBuffer(vbo_Position_Pyramid);
        vbo_Position_Pyramid = null;
    }

    if (vao_Pyramid) {
        gl.deleteVertexArray(vao_Pyramid);
        vao_Pyramid = null;
    }



    

    
}





