////////////////////////////INTEGRAÇÃO///////////////////////////////

const getCSV = async () => {
    try {
        //lê o arquivo .csv no servidor
        const target = `https://SOME_DOMAIN.com/data/csv/[FILENAME].csv`;


        const res = await fetch(target, {
            method: 'get',
            headers: {
                'content-type': 'text/csv;charset=UTF-8',
            }
        });

        if (res.status === 200) {
            const data = await res.text();
            console.log(data);

        } else {
            console.log(`Error code ${res.status}`);
        }
    } catch (err) {
        console.log(err)
    }
}



// PEGA TEMPERATURA E UMIDADE RECEBIDAS DO ESP
//https://docs.google.com/spreadsheets/d/1jP9yXgPAHUr0RjDQ1sTmqObHoKnP-r0I6hto-3vMw80/edit
async function getEspReadings(){
    const sheetId = '1jP9yXgPAHUr0RjDQ1sTmqObHoKnP-r0I6hto-3vMw80'
    const base = `https://docs.google.com/spreadsheets/d/${sheetId}/gviz/tq?`;
    const sheetName = 'Sheet 1';
    const query = encodeURIComponent('Select *')
    const url2 = `${base}&sheet=${sheetName}&tq=${query}`
    const appendedJSON = []
    let rep = await fetch(url2);
    rep = await rep.text();

    const jsonData = JSON.parse(rep.slice(47, -2))
    //Pegando a ultima linha da planilha e transformando em uma array 
    var newJSON = jsonData.table.rows[jsonData.table.rows.length - 1]

    // Pegando os valores do json das temperaturas e umidades definidas pelo user 
    var lastDate = newJSON.c[0].v
    var lastTemp = newJSON.c[2].v
    var lastUmi = newJSON.c[3].v

    //Array com os valores de temperatura e umidade para serem enviados para o front
    let lastMeasures = [lastTemp,lastUmi,lastDate]
    //console.log(lastMeasures, "FUNC GET ESP READINGS");

    return lastMeasures;

}

//Função para recer os dados de temperatura max e min que vêm do front embarcado
async function getInputTemp(){
    const sheetId = '1L7lRsg9MDNTPqTWmi661626ncFGmp9wtVwGpQj_ZS4Y'
    const base = `https://docs.google.com/spreadsheets/d/${sheetId}/gviz/tq?`;
    const sheetName = 'Página 1';
    const query = encodeURIComponent('Select *')
    const url2 = `${base}&sheet=${sheetName}&tq=${query}`
    const appendedJSON = []
    let rep = await fetch(url2);
    rep = await rep.text();

    const jsonData = JSON.parse(rep.slice(47, -2))
    //Pegando a ultima linha da planilha e transformando em uma array 
    var newJSON = jsonData.table.rows[jsonData.table.rows.length - 1]

    // Pegando os valores do json das temperaturas e umidades definidas pelo user 
    var tempMin = newJSON.c[3].v
    var tempMax = newJSON.c[2].v

    //Colocando esses valores dentro da array criada para armazenar tais valores
    let tempInputs = [tempMin,tempMax]
    //console.log(tempInputs, "PEGANDO INPUTS TEMP");

    return tempInputs;

}


//Função para recer os dados de umidade max e min que vêm do front embarcado
// Link da planilha https://docs.google.com/spreadsheets/d/171ubYrqZ7Vq9fynssYJwAk017Jty2fHZbe9cAV_bUIY/edit#gid=0
async function getInputUmi(){
    const sheetId = '171ubYrqZ7Vq9fynssYJwAk017Jty2fHZbe9cAV_bUIY'
    const base = `https://docs.google.com/spreadsheets/d/${sheetId}/gviz/tq?`;
    const sheetName = 'Página 1';
    const query = encodeURIComponent('Select *')
    const url2 = `${base}&sheet=${sheetName}&tq=${query}`
    const appendedJSON = []
    let rep = await fetch(url2);
    rep = await rep.text();

    const jsonData = JSON.parse(rep.slice(47, -2))
    //Pegando a ultima linha da planilha e transformando em uma array 
    var newJSON = jsonData.table.rows[jsonData.table.rows.length - 1]

    // Pegando os valores do json das temperaturas e umidades definidas pelo user 
    var umiMin = newJSON.c[3].v
    var umiMax = newJSON.c[2].v

    //Fazendo o array
    let umiInputs = [umiMin, umiMax]

    return umiInputs;


    }


/////////////////////////////FUNÇÕES PARA O FRONT/////////////////////////////////////


// Função que adiciona um scroll de rolagem vertical quando o número de filhos for maior que 8
// Irá funcionar caso seja implementada a função de adicionar novas estufas
function stovesCount() {

    let stoves = document.querySelector('.grid');

    if (stoves.childElementCount > 8) {

        stoves.style.overflowY = 'scroll';
    }
}


//Função para enviar o valor da temperatura para a aba da estufa, 
// Altera a cor da barra de "progresso" e a mensagem de recomendação de acordo com a leitura
async function temperatureProgressBar() {
    
    //Variáveis para o html
    let bar = document.querySelector('.temperature-bar');
    let text = document.querySelector('.temperature-data .recommendation');
    let temperatureValue = document.querySelector('.t-progress-bar p');
    let temperatureCardValue = document.querySelector('.card-temp');
    let tempCardBar = document.querySelector('.temperature_color');

    //Recebendo o retorno da função getEsp (array)
    let espArrayT = await getEspReadings();
    //(espArrayT)

    //Pegando a última medição
    let temp = espArrayT[0];
    //console.log(temp, "pegando o temp da array");

    //Recebendo o retorno da função e pegando o input de min e max
    let tMinMax = await getInputTemp();
    let tMin = tMinMax[0];
    let tMax = tMinMax[1];

    
    //pegando as classes para enviar valor min e max
    let t_min = document.querySelector('.t-min');
    let t_max = document.querySelector('.t-max');
    
    //Passando os valores da array para uma variável
    let minTemperature = tMin;
    let maxTemperature = tMax;
    
    //enviando os dados para o front
    t_min.innerHTML = minTemperature;
    t_max.innerHTML = maxTemperature;
    
    
    //Variável usada na função dos alertas
    let value_temp;


    //variáveis para definir as cores
    let color;
    let red = '#c74a34';
    let green = '#99cc66';
    let yellow = '#e5a812';

    //fazendo as condições
    if (temp > 0 && temp <= (minTemperature * 0.95)) {

        color = red;
        text.innerHTML = "Alerta: Temperatura BAIXA.";
        value_temp = 1;
    }
    else if (temp > (minTemperature * 0.95) && temp < (maxTemperature * 1.03)) {

        color = green;
        text.innerHTML = "Temperatura dentro do ideal.";
        value_temp = 0;
    }
    else if (temp >= (maxTemperature * 1.03) && temp < (maxTemperature * 1.05)) {

        color = yellow;
        text.innerHTML = "Alerta: Temperatura ALTA, abrir 50% das janelas laterais.";
        value_temp = -1;
    }
    else if (temp >= (maxTemperature * 1.05) && temp < (maxTemperature * 1.1)) {

        color = yellow;
        text.innerHTML = "Alerta: Temperatura ALTA, abrir 100% das janelas laterais";
        value_temp = -1;
    }
    else if (temp >= (maxTemperature * 1.1)) {

        color = red;
        text.innerHTML = "Alerta: Temperatura ALTA, abrir janelas zenitais e laterais.";
        value_temp = 1;
    }
    else if (temp == 0 || temp == undefined || temp == null) {
        color = "#A9a9a9";
        text.innerHTML = "Erro: falha no recebimento dos dados. Possíveis erros: perda de energia; falha de conexão; dados nulos; componente danificado.";
        document.querySelector('.confirm-div p').innerHTML = 'Emitir alerta de inatividade'
        let confirmButton = document.querySelector('.confirm-button');
        confirmButton.innerHTML = 'Emitir alerta';
        confirmButton.style.backgroundColor = 'rgb(199, 74, 52)';
        confirmButton.addEventListener('mouseover', () => {
            confirmButton.style.backgroundColor = '#fff';
            confirmButton.style.color = 'rgb(199, 74, 52)';
            confirmButton.style.transition = 0.4;
        })
        confirmButton.addEventListener('mouseout', () => {
            confirmButton.style.color = '#fff';
            confirmButton.style.backgroundColor = 'rgb(199, 74, 52)';
        })
        value_temp = 1;
    }

    //Enviando os dados para o front
    bar.style.backgroundColor = color;
    temperatureValue.innerHTML = `${temp} °C`;
    tempCardBar.style.backgroundColor = color;
    temperatureCardValue.innerHTML = `${temp} °C`;

    //Criando um objeto para guardar o value_temp e ser usado para outros fins
    values = {
        humid: null,
        temp: null
    }

    //Guardando o valor no objeto
    values.temp = value_temp;
}

//Função para enviar o valor da umidade para a aba da estufa, 
// Altera a cor da barra de "progresso" e a mensagem de recomendação de acordo com a leitura
async function humidityProgressBar() {
    
    //Variáveis para o html
    let humidityBar = document.querySelector('.humidity-bar');
    let text = document.querySelector('.humidity-data .recommendation');
    let humidityValue = document.querySelector('.u-progress-bar p');
    let humidityCardValue = document.querySelector('.card-umi');
    let humCardBar = document.querySelector('.humidity_color');
    let u_min = document.querySelector('.u-min');
    let u_max = document.querySelector('.u-max')

    //Recebendo o retorno da função getEsp (array)
    let espArrayU = await getEspReadings();
    //console.log(espArrayU);

    //Pegando a última medição
    let humi = espArrayU[1];
    //console.log(humi, "pegando o umi da array");

    //Recebendo o retorno da função e pegando o input de min e max
    let uMinMax = await getInputUmi();
    let uMin = uMinMax[0];
    let uMax = uMinMax[1];

    //Passando os valores da array para uma variável
    let minUmidity = uMin;
    let maxUmidity = uMax;

    //Enviando os valores para o front
    u_min.innerHTML = minUmidity;
    u_max.innerHTML = maxUmidity;



    //Variável usada na função dos alertas
    let value_hum;

    //Definindo as cores
    let color;
    let red = '#c74a34';
    let green = '#99cc66';

    //Fazendo as condições
    if (humi > 0 && humi <= (minUmidity * 0.95)) {

        color = red;
        text.innerHTML = "Alerta: Umidade Relativa BAIXA.";
        value_hum = 1;
    }
    else if (humi > (minUmidity * 0.95) && humi < maxUmidity) {

        color = green;
        text.innerHTML = "Alerta: Umidade Relativa dentro do ideal.";
        value_hum = 0;
    }
    else if (humi >= (maxUmidity)) {

        color = red;
        text.innerHTML = "Alerta: Umidade Relativa ALTA.";
        value_hum = 1;
    }

    //Erro no recebimento dos dados
    else if (humi == 0 || humi == undefined || humi == null) {
        color = "#A9a9a9";
        value_hum = 1;
        text.innerHTML = "Erro: falha no recebimento dos dados. Possíveis erros: perda de energia; falha de conexão; dados nulos; componente danificado.";
        document.querySelector('.confirm-div p').innerHTML = 'Emitir alerta de inatividade'
        let confirmButton = document.querySelector('.confirm-button');
        confirmButton.innerHTML = 'Emitir alerta';
        confirmButton.style.backgroundColor = 'rgb(199, 74, 52)';
        confirmButton.addEventListener('mouseover', () => {
            confirmButton.style.backgroundColor = '#fff';
            confirmButton.style.color = 'rgb(199, 74, 52)';
            confirmButton.style.transition = 0.4;
        })
        confirmButton.addEventListener('mouseout', () => {

            confirmButton.style.color = '#fff';
            confirmButton.style.backgroundColor = 'rgb(199, 74, 52)';
        })
    }

    //Enviando os dados para o front
    humidityValue.innerHTML = `${humi} %`;
    humidityCardValue.innerHTML = `${humi} %`;
    humidityBar.style.backgroundColor = color;
    humCardBar.style.backgroundColor = color;

    //Guardando o value_hum no objeto
    values.humid = value_hum;
    
    //Mandando o objeto para a nossa função de esconder botão de confirmação
    hideConfirmationButton(values);
    //Mandando para a função que altera o estado dos triângulos
    changeStoveTriangle(values);

    //Zerando o objeto para não dar conflito
    values = {
        humid: null,
        temp: null
    }
}


//Faz os status das estudas aparecerem como inativos caso receba valores com erro
async function stoveStatus() {

    let states = document.getElementsByClassName('state');

    let espArray = await getEspReadings();
    let temp = espArray[0];
    let humi = espArray[1];

    for (n of states) {
        if (humi == 0 || humi == undefined || humi == null || temp == 0 || temp == undefined || temp == null) {
            n.innerHTML = 'Inativo'
        }
        else {
            n.innerHTML = 'Ativo'
        }
    }
}

// Função para atualizar o horário da última medição na descrição da estufa/dispositivo
async function showDate() {

    //Variáveis html
    let updateDate = document.querySelector('.hour');
    let lastDateHeader = document.querySelector('.date');
  
    //Recebendo retorno da função getEsp (array) e pegando última data
    let espArrayD = await getEspReadings();
    let date = espArrayD[2];

    //Inserindo no html
    updateDate.innerHTML = date;
    lastDateHeader.innerHTML = `Última medição: ${date}`;
}

//Função que abre a aba de descrição da estufa 
function StoveDesc() {
    var desc = document.querySelector('.right-side-column')
    desc.style.display = 'flex';
    document.querySelector('.teste-butt').style.boxShadow = '10px 10px 10px blue'
}

//Função que fecha a aba de descrição da estufa
function closeDescPage() {
    var desc = document.querySelector('.right-side-column')
    desc.style.display = 'none';
    document.querySelector('.teste-butt').style.boxShadow = 'none';
}

//Função que irá esconder o botão de enviar confirmação de abertura quando a umi e temp estiverem dentro do padrão
function hideConfirmationButton(values) {
    var div_button = document.querySelector('.confirm-div')

    if (values.temp == 0 && values.humid == 0) {
        div_button.style.display = 'none'
    }
    else {
        div_button.style.display = 'flex'
    }
}

// Função para abrir o modal de editar estufa/dispositivo
function editModal() {
    let modal = document.querySelector('.edit-modal-background');
    modal.style.display = 'flex';
    let container = document.querySelector('.container');
    let rightSideDiv = document.querySelector('.right-side-column');
    container.style.filter = 'blur(2px)';
    rightSideDiv.style.filter = 'blur(2px)';
}

// Função para fechar o modal de editar estufa/dispositivo
function closeEditModal() {
    let container = document.querySelector('.container');
    let rightSideDiv = document.querySelector('.right-side-column');
    let modal = document.querySelector('.edit-modal-background');
    modal.style.display = 'none';
    container.style.filter = 'none';
    rightSideDiv.style.filter = 'none';
}

// Função que troca o tipo de triângulo que aparece no card e na descrição da estufa
// Depende do estado da temperatura e umidade
function changeStoveTriangle(values) {

    console.log(values.temp, "valor temperatura")
    console.log(values.humid, "valor umidade")
    
    let card_triangle = document.querySelector('.stove .caution');
    let desc_triangle = document.querySelector('.desc-triangle');
    
    //Definindo cores
    let attention = "../images/attention_triangle.svg"; //amarelo
    let warning = "../images/warning_triangle.svg" //vermelho


    //Fazendo condições
    if (values.temp == 1 || values.humid == 1) {
        color = warning;

        card_triangle.setAttribute('src', color);
        desc_triangle.setAttribute('src', color);

    }
    else if (values.temp == -1) {
        color = attention;

        card_triangle.setAttribute('src', color);
        desc_triangle.setAttribute('src', color);
    }
    else if (values.temp == 0 && values.humid == 0) {
        card_triangle.style.display = 'none'
        desc_triangle.style.display = 'none'

    }
}

//função que exibe um alert para confirmação de envio de alerta
function sendConfirm() {
    setTimeout(() => {
        alert('Alerta enviado com sucesso!');
    }, 1000);
}

//função que exibe um alert para confirmação de download do csv
function downloadCSV() {
    setTimeout(() => {
        alert('Arquivo csv baixado com sucesso!');
    }, 1000);
}

function sendIP(){
    alert("Endereço para configuração: 192.168.4.1 \nLembre-se se estar conectado à rede do dispositivo. \n\nNome: Koalyptus \nSenha: koalaslyptus");
}



//Aqui alteramos o tempo para atualizar 
var timer = 5 * 1000;

//Aqui atualizamos as funções de tempo em tempo
let intervalId = window.setInterval(function(){
    getEspReadings();
    temperatureProgressBar();
    humidityProgressBar();
    getInputTemp();
    getInputUmi();
    stoveStatus();
    showDate();
    //changeStoveTriangle();
}, timer);