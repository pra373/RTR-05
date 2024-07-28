// JavaScript source code

//main function

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_original_width;

var canvas_original_height;

var angleP = 0.0;

//WebGL related variables

const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_NORMAL: 1
};

var shaderProgramObject = null;
var vao = null;
var vbo = null;

var vbo_Normal = null;
var mvpMatrixUniform;

var perspectiveProjectionMatrix;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

// Uniforms

var modelMatrixUniform = 0;
var viewMatrixUniform = 0;
var projectionMatrixUniform = 0;

var lightDiffuseUniform = new Array(2);
var lightAmbientUniform = new Array(2);
var lightSpecularUniform = new Array(2);
var lightPositionUniform = new Array(2);

var materialDiffuseUniform = 0;
var materialAmbientUniform = 0;
var materialSpecularUniform = 0;
var materialShininessUniform = 0;

var keyPressUniform = 0;

var bLightingEnable = false;

var bAnimationEnable = false;

//material properties

var materialAmbient = new Float32Array([0.0, 0.0, 0.0]);
var materialDiffuse = new Float32Array([ 1.0, 1.0, 1.0]);
var materialSpecular = new Float32Array([ 1.0, 1.0, 1.0]);
var materialShininess = 128.0;


//lights properties

var light1Position = new Float32Array(4);
var light1Diffuse = new Float32Array(3);
var light1Specular = new Float32Array(3);
var light1Ambient = new Float32Array(3);

var light2Position = new Float32Array(4);
var light2Diffuse = new Float32Array(3);
var light2Specular = new Float32Array(3);
var light2Ambient = new Float32Array(3);
                 

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

        case 65:
        case 97:
            if (bAnimationEnable == false)
            {
                bAnimationEnable = true;
            }
            else
            {
                bAnimationEnable = false;
            }

            break;

        case 76:
        case 108:
            if (bLightingEnable == false)
            {
                bLightingEnable = true;
            }
            else
            {
                bLightingEnable = false;
            }

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
            "in vec4 aPosition;\n"+
		    "in vec3 aNormal;\n"+
            "uniform mat4 uModelMatrix;\n" +
            "uniform mat4 uViewMatrix;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform vec3 ulightAmbient[2];\n" +
            "uniform vec3 ulightDiffuse[2];\n" +
            "uniform vec3 ulightSpecular[2];\n" +
            "uniform vec4 ulightPosition[2];\n" +
            "uniform vec3 uMaterialAmbient;\n" +
            "uniform vec3 uMaterialDiffuse;\n" +
            "uniform vec3 uMaterialSpecular;\n" +
            "uniform float uMaterialShininess;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec3 oPhongADSLight;\n" +
            "void main(void)\n" +
            "{\n" +
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
            "vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);\n" +
            "vec3 lightAmbient[2];\n" +
            "vec3 lightDirection[2];\n" +
            "vec3 lightDiffuse[2];\n" +
            "vec3 reflectionVector[2];\n" +
            "vec3 lightSpecular[2];\n" +
            "for(int i = 0; i < 2; i++)\n" +
            "{\n" +
            "lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n" +
            "lightDirection[i] = normalize(vec3 (ulightPosition[i] - eyeCoordinates));\n" +
            "lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i], transformedNormals),0.0);\n" +
            "reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n" +
            "vec3 viewerVector = normalize (- eyeCoordinates.xyz);\n" +
            "lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], viewerVector), 0.0),uMaterialShininess);\n" +
            "oPhongADSLight = oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n" +
            "}\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "oPhongADSLight = vec3( 0.0, 0.0, 0.0);\n" +
            "}\n" +
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
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
            "in vec3 oPhongADSLight;\n"+
		    "uniform highp int uKeyPress;\n"+
            "out vec4 FragColor;\n"+
            "void main(void)\n"+
            "{\n"+
            "if(uKeyPress == 1)\n"+
            "{\n"+
            "  FragColor = vec4 (oPhongADSLight, 1.0);\n"+
            "}\n"+
            "else\n"+
            "{\n"+
            "  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"+
            "}\n"+
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

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false)
    {
        var error = gl.getProgramInfoLog(shaderProgramObject);

        if (error.length > 0)
        {
            var log = "shader program linking error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("shader program linked Successfully !\n");
    }

    
    //mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");

    modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uModelMatrix");

    viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uViewMatrix");

    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uProjectionMatrix");

    lightAmbientUniform[0] = gl.getUniformLocation(shaderProgramObject, "ulightAmbient[0]");

    lightDiffuseUniform[0] = gl.getUniformLocation(shaderProgramObject, "ulightDiffuse[0]");

    lightSpecularUniform[0] = gl.getUniformLocation(shaderProgramObject, "ulightSpecular[0]");

    lightPositionUniform[0] = gl.getUniformLocation(shaderProgramObject, "ulightPosition[0]");

    lightAmbientUniform[1] = gl.getUniformLocation(shaderProgramObject, "ulightAmbient[1]");

    lightDiffuseUniform[1] = gl.getUniformLocation(shaderProgramObject, "ulightDiffuse[1]");

    lightSpecularUniform[1] = gl.getUniformLocation(shaderProgramObject, "ulightSpecular[1]");

    lightPositionUniform[1] = gl.getUniformLocation(shaderProgramObject, "ulightPosition[1]");


    materialAmbientUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialAmbient");

    materialDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialDiffuse");


    materialSpecularUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialSpecular");

    materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialShininess");

    keyPressUniform = gl.getUniformLocation(shaderProgramObject, "uKeyPress");



    //geometry position array declaration

    var pyramidPosition = new Float32Array
        ([
            // front
            0.0, 1.0, 0.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0,

            // right
            0.0, 1.0, 0.0,
            1.0, -1.0, 1.0,
            1.0, -1.0, -1.0,

            // back
            0.0, 1.0, 0.0,
            1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0,

            // left
            0.0, 1.0, 0.0,
            -1.0, -1.0, -1.0,
            -1.0, -1.0, 1.0
        ]);

    var pyramidNormal = new Float32Array
        ([
            // front
            0.000000, 0.447214, 0.894427, // front-top
            0.000000, 0.447214, 0.894427, // front-left
            0.000000, 0.447214, 0.894427, // front-right

            // right			    
            0.894427, 0.447214, 0.000000, // right-top
            0.894427, 0.447214, 0.000000, // right-left
            0.894427, 0.447214, 0.000000, // right-right

            // back
            0.000000, 0.447214, -0.894427, // back-top
            0.000000, 0.447214, -0.894427, // back-left
            0.000000, 0.447214, -0.894427, // back-right

            // left
            -0.894427, 0.447214, 0.000000, // left-top
            -0.894427, 0.447214, 0.000000, // left-left
            -0.894427, 0.447214, 0.000000 // left-right
        ]);



    //vao

    vao = gl.createVertexArray();

    gl.bindVertexArray(vao);

    //vbo

    vbo = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);

    gl.bufferData(gl.ARRAY_BUFFER, pyramidPosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo


    //vbo

    vbo_Normal = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Normal);

    gl.bufferData(gl.ARRAY_BUFFER, pyramidNormal, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);
        

    //depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);
    

    // set clear color

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    //initialize color Properties

    // ************************** Light 1 Properties ****************************

    //light 1 ambient 

    light1Ambient[0] = 0.0;
    light1Ambient[1] = 0.0;
    light1Ambient[2] = 0.0;

    //light 1 diffuse

    light1Diffuse[0] = 1.0;
    light1Diffuse[1] = 0.0;
    light1Diffuse[2] = 0.0;

    //light 1 specular

    light1Specular[0] = 1.0;
    light1Specular[1] = 0.0;
    light1Specular[2] = 0.0;

    //light 1 position

    light1Position[0] = -2.0;
    light1Position[1] = 0.0;
    light1Position[2] = 0.0;
    light1Position[3] = 1.0;

    // ************************** Light 2 Properties ****************************

    //light 2 ambient 

    light2Ambient[0] = 0.0;
    light2Ambient[1] = 0.0;
    light2Ambient[2] = 0.0;

    //light 2 diffuse

    light2Diffuse[0] = 0.0;
    light2Diffuse[1] = 0.0;
    light2Diffuse[2] = 1.0;

    //light 2 specular

    light2Specular[0] = 0.0;
    light2Specular[1] = 0.0;
    light2Specular[2] = 1.0;

    //light 2 position

    light2Position[0] = 2.0;
    light2Position[1] = 0.0;
    light2Position[2] = 0.0;
    light2Position[3] = 1.0;


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

    //transformation

    var modelViewMatrix = mat4.create();

    var viewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var rotationMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -4.0]);

    mat4.rotate(rotationMatrix, rotationMatrix, angleP, [0, 1, 0]);

    mat4.multiply(modelViewMatrix, translationMatrix, rotationMatrix);

    // gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);



    gl.uniformMatrix4fv(modelMatrixUniform, false, modelViewMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);


    //send uniforms

    if (bLightingEnable == true)
    {
        gl.uniform1i(keyPressUniform, 1);

        gl.uniform3fv(lightAmbientUniform[0], light1Ambient);

        gl.uniform3fv(lightDiffuseUniform[0], light1Diffuse);

        gl.uniform3fv(lightSpecularUniform[0], light1Specular);

        gl.uniform4fv(lightPositionUniform[0], light1Position);

        gl.uniform3fv(lightAmbientUniform[1], light2Ambient);

        gl.uniform3fv(lightDiffuseUniform[1], light2Diffuse);

        gl.uniform3fv(lightSpecularUniform[1], light2Specular);

        gl.uniform4fv(lightPositionUniform[1], light2Position);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);

        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);

        gl.uniform3fv(materialSpecularUniform, materialSpecular);

        gl.uniform1f(materialShininessUniform, materialShininess);

    }
    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }





    gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLES, 0, 12);

    gl.bindVertexArray(null);

    gl.useProgram(null);

    if (bAnimationEnable == true)
    {
        update();
    }

   

    //double buffering

    requestAnimationFrame(display, canvas);
    
}

function update()
{

    //Triangle Rotation

    if (angleP < 360.0)
    {
        angleP = angleP + 0.007;
    }

    else
    {
        angleP = -360.0;
    }

    //code
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

    if (vbo_Normal)
    {
        gl.deleteBuffer(vbo_Normal);
        vbo_Normal = null;
    }

    if (vao)
    {
        gl.deleteVertexArray(vao);
        vao = null;
    }



    

    
}





