// ====================================
// Fases extraídas do seu entrada.txt
// ====================================
const fases = [
    {
        letras: "RAAUG",
        palavras: ["RUGA", "GRAU", "RUA", "AGUA", "AGUAR"]
    },
    {
        letras: "IALMT",
        palavras: ["LATIM", "LIMA", "TIL", "MIL", "TIA", "TAL"]
    },
    {
        letras: "OTARIA",
        palavras: ["RATO", "TORA", "RIA", "RITA", "ROTA", "ATOR"]
    },
    {
        letras: "AUPLOD",
        palavras: ["LUPA", "PULA", "LUA", "DUPLO", "DOA", "LUDO"]
    }
];

let faseAtual = 0;
let palavraAtual = "";
let pontuacao = 0;
let usadas = [];

// HTML elements
const grid = document.getElementById("letter-grid");
const currentWord = document.getElementById("current-word");
const wordList = document.getElementById("word-list");
const scoreEl = document.getElementById("score");
const phaseTitle = document.getElementById("phase-title");
const nextPhaseBtn = document.getElementById("next-phase-btn");

// ===============================
// INICIAR FASE
// ===============================
function carregarFase() {
    const fase = fases[faseAtual];

    phaseTitle.textContent = `Fase ${faseAtual + 1}`;
    palavraAtual = "";
    usadas = Array(fase.letras.length).fill(false);

    // Grid de letras
    grid.innerHTML = "";
    for (let i = 0; i < fase.letras.length; i++) {
        const tile = document.createElement("div");
        tile.className = "letter-tile";
        tile.textContent = fase.letras[i];

        tile.onclick = () => selecionarLetra(i);
        grid.appendChild(tile);
    }

    // Lista de palavras
    wordList.innerHTML = "";
    fase.palavras.forEach(p => {
        const li = document.createElement("li");
        li.textContent = "_".repeat(p.length);
        li.className = "word-hidden";
        wordList.appendChild(li);
    });

    nextPhaseBtn.style.display = "none";
}

// ===============================
// Selecionar letra
// ===============================
function selecionarLetra(index) {
    if (usadas[index]) return;

    usadas[index] = true;
    palavraAtual += fases[faseAtual].letras[index];
    currentWord.textContent = palavraAtual;

    grid.children[index].classList.add("used");
}

// ===============================
// Limpar palavra atual
// ===============================
document.getElementById("clear-btn").onclick = () => {
    palavraAtual = "";
    usadas = usadas.map(() => false);

    currentWord.textContent = "";

    for (let tile of grid.children) {
        tile.classList.remove("used");
    }
};

// ===============================
// Enviar palavra
// ===============================
document.getElementById("submit-btn").onclick = () => {
    const fase = fases[faseAtual];
    const index = fase.palavras.indexOf(palavraAtual);

    if (index === -1) {
        alert("❌ Palavra inválida! -10 pontos");
        pontuacao -= 10;
    } else {
        const li = wordList.children[index];

        if (li.classList.contains("found")) {
            alert("⚠️ Palavra já encontrada! -10 pontos");
            pontuacao -= 10;
        } else {
            alert("✅ Palavra encontrada! +100 pontos");
            li.textContent = fase.palavras[index];
            li.classList.add("found");
            pontuacao += 100;
        }
    }

    scoreEl.textContent = `Pontuação: ${pontuacao}`;
    palavraAtual = "";
    currentWord.textContent = "";

    usadas = usadas.map(() => false);
    for (let tile of grid.children) {
        tile.classList.remove("used");
    }

    // Checar fim de fase
    const todas = [...wordList.children].every(li =>
        li.classList.contains("found")
    );

    if (todas) {
        alert("🎉 Fase completa! +25 pontos");
        pontuacao += 25;
        scoreEl.textContent = `Pontuação: ${pontuacao}`;

        nextPhaseBtn.style.display = "block";
    }
};

// ===============================
// Próxima fase
// ===============================
nextPhaseBtn.onclick = () => {
    faseAtual++;

    if (faseAtual >= fases.length) {
        alert("🏆 Você zerou o jogo!");
        return;
    }

    carregarFase();
};

// Inicializar
carregarFase();