
var statusElement = document.getElementById('status');
var progressElement = document.getElementById('progress');
var spinnerElement = document.getElementById('spinner');

var Module = {
    noInitialRun: true,
    print: (function () {
        var element = document.getElementById('output');
        if (element) element.value = ''; // clear browser cache
        return (...args) => {
            var text = args.join(' ');
            // These replacements are necessary if you render to raw HTML
            //text = text.replace(/&/g, "&amp;");
            //text = text.replace(/</g, "&lt;");
            //text = text.replace(/>/g, "&gt;");
            //text = text.replace('\n', '<br>', 'g');
            console.log(text);
            if (element) {
                element.value += text + "\n";
                element.scrollTop = element.scrollHeight; // focus on bottom
            }
        };
    })(),
    canvas: (() => {
        var canvas = document.getElementById('canvas');

        // As a default initial behavior, pop up an alert when webgl context is lost. To make your
        // application robust, you may want to override this behavior before shipping!
        // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
        canvas.addEventListener("webglcontextlost", (e) => { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);

        return canvas;
    })(),
    setStatus: (text) => {
        Module.setStatus.last ??= { time: Date.now(), text: '' };
        if (text === Module.setStatus.last.text) return;
        var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
        var now = Date.now();
        if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
        Module.setStatus.last.time = now;
        Module.setStatus.last.text = text;
        if (m) {
            text = m[1];
            progressElement.value = parseInt(m[2]) * 100;
            progressElement.max = parseInt(m[4]) * 100;
            progressElement.hidden = false;
            spinnerElement.hidden = false;
        } else {
            progressElement.value = null;
            progressElement.max = null;
            progressElement.hidden = true;
            if (!text) spinnerElement.hidden = true;
        }
        statusElement.innerHTML = text;
    },
    totalDependencies: 0,
    monitorRunDependencies: (left) => {
        this.totalDependencies = Math.max(this.totalDependencies, left);
        Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
    }
};
//Module.setStatus('Downloading...');
//window.onerror = () => {
//    Module.setStatus('Exception thrown, see JavaScript console');
//    spinnerElement.style.display = 'none';
//    Module.setStatus = (text) => {
//        if (text) console.error('[post-exception status] ' + text);
//    };
//};

const games = document.querySelectorAll('.member');
var currentRom = [];
var frequency = 0;
var playing = false;
const resetButton = document.querySelector('#reset');

function loadRom(rom) {
    Module.ccall("loadRom", null, ['string', 'number'], [rom, 5]);
}
function startGame() {
    Module.ccall("main", null, null, null);
}
function stopGame() {
    Module.ccall("stop", null, null, null);
}
Module["onRuntimeInitialized"] = function () {

    games.forEach(game => {
        game.addEventListener('click', function (event) {
            var activeGame = document.getElementById('active');
            var selected = "roms/" + (event.target.innerHTML).replace(/\s+/g, '').toLowerCase() + ".ch8";
            if (activeGame) {
                activeGame.id = '';
                stopGame();
            }
            game.id = 'active'
            setInstructions();
            loadRom(selected);
            startGame();
        })
    })

    resetButton.addEventListener('click', function (event) {
        console.log("reset");
        activeGame = document.getElementById('active');
        if (!activeGame) {
            console.log("There is no active game")
            return;
        }
        let currentGame = "roms/" + (activeGame.innerHTML).replace(/\s+/g, '').toLowerCase() + ".ch8";
        console.log(currentGame)
        stopGame();
        loadRom(currentGame);
        startGame();
    })
}

function setInstructions() {
    activeGame = document.getElementById('active');
    switch (activeGame.innerHTML) {
        case ("Tetris"):
            document.querySelector('.instructions').innerHTML = "<p>Q: ROTATE PIECE<br>W: MOVE LEFT<br>E: MOVE RIGHT</p>"
            break;
        case ("Brick Break"):
            document.querySelector('.instructions').innerHTML = "<p>Q: MOVE LEFT<br>E: MOVE RIGHT</p>"
            break;
        case ("Outlaw"):
            document.querySelector('.instructions').innerHTML = "<p>W: MOVE UP<br>A: LEFT<br>S: MOVE DOWN<br>D:MOVE RIGHT<br>E: SHOOT</p>"
            break;
        case ("Space Jam"):
            document.querySelector('.instructions').innerHTML = "<p>W: MOVE UP<br>A: LEFT<br>S: MOVE DOWN<br>D:MOVE RIGHT</p>"
            break;
        case ("Blitz"):
            document.querySelector('.instructions').innerHTML = "<p>PRESS W TO BEGIN<br>W: SHOOT</p>"
            break;
        case ("Merlin"):
            document.querySelector('.instructions').innerHTML = "<p>Q:SELECT TOP LEFT<br>W: SELECT TOP RIGHT<br>A: SELECT BOTTOM LEFT<br>S: SELECT BOTTOM RIGHT</p>"
            break;
        case ("Pong"):
            document.querySelector('.instructions').innerHTML = "<p>1: MOVE UP<br>Q: MOVE DOWN</p>"
            break;
        case ("Tapeworm"):
            document.querySelector('.instructions').innerHTML = "<p>V: BEGIN GAME<br>2: GO UP<br>Q: GO LEFT<br>S: GO DOWN<br>E: GO RIGHT</p>"
            break;
        case ("Soccer"):
            document.querySelector('.instructions').innerHTML = "<p>1: MOVE UP<br>Q: MOVE DOWN</p>"
            break;
    }

}


