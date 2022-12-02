/////////////////////////////INTEGRAÇÃO///////////////////////////////


// Pegando o link do Google Sheets
var id = "1jP9yXgPAHUr0RjDQ1sTmqObHoKnP-r0I6hto-3vMw80";
var gid = "0";
var url =
  "https://docs.google.com/spreadsheets/d/" +
  id +
  "/gviz/tq?tqx=out:json&tq&gid=" +
  gid;

// Dando slice na planilha para pegar os últimos dados de leitura (enviados pelo esp32)  
fetch(url)
  .then((response) => response.text())
  .then(
    (data) =>
      (getDataToJson(data.slice(47, -2)))
  );


//Função transforma a linha (que pegamos no fetch) em json
// Depois, separamos cada linha do json em uma variável
// Criamos uma variável para cada dado = data, temperatura e umidade
function getDataToJson(jsonString) {
  var json = JSON.parse(jsonString);
  const lastRegister = json.table.rows[json.table.rows.length - 1]; 
  var lastDate = lastRegister.c[0].v;
  var lastTemperature = lastRegister.c[2].v;
  var lastHumidity = lastRegister.c[3].v;

  //Enviando os dados para cada uma das funções - responsáveis por exibir no front
  temperatureProgressBar(lastTemperature);
  humidityProgressBar(lastHumidity);
  showDate(lastDate);
  makeInactive(lastTemperature, lastHumidity);


}


/////////////////////////////FUNÇÕES PARA O FRONT/////////////////////////////////////

//Variável que será usada na função para efeito de selecionado
//Ainda em construção
var stoveID;



// Função que adiciona um scroll de rolagem vertical quando o número de filhos for maior que 8
function stovesCount() {

    let stoves = document.querySelector('.grid');

    if (stoves.childElementCount > 8) {

        stoves.style.overflowY = 'scroll';
    }

}

// Função de filtragem das estufas pelo input de texto
//Falta finalizar
function filter() {

    let filterInput = document.querySelector('.filter-input')
    console.log(filterInput.value)

}



//Função para enviar o valor da temperatura para a aba da estufa, 
// Altera a cor da barra de "progresso" e a mensagem de recomendação de acordo com a leitura
function temperatureProgressBar(lastTemperature) {

    let bar = document.querySelector('.temperature-bar');
    let text = document.querySelector('.temperature-data .recommendation');
    let temperatureValue = document.querySelector('.t-progress-bar p');
    let tempCardBar = document.querySelector('.temperature_color');
    
    //Essa variável pega o valor real (último lido) de temperatura, enviado pela função 
    let temp = lastTemperature;

    /////////////CASOS DE TESTE TEMPERATURA://///////////
    // Para testar, comente a variável let temp = lastTemperature; e descomente a que você deseja testar
    
    //let temp = 25; //temp baixa
    //let temp = 32; //temp dentro do ideal 
    //let temp = 37.6; //abrir 50%
    //let temp = 38; //abrir 100%
    //let temp = 40; //laterais e zenitais


    //variáveis para definir as cores
    let color;
    let red = '#c74a34';
    let green = '#99cc66';
    let yellow = 'e5a812';


    //fazendo as condições
    if (temp > 0 && temp <= 26.6) {
        
        color = red;
        text.innerHTML = "Alerta: Temperatura BAIXA.";
        value_temp = 1;

    }

    else if (temp > 26.6 && temp < 37.08) {

        color = green;
        text.innerHTML = "Temperatura dentro do ideal.";
        value_temp = 0;

    }
    else if (temp >= 37.08 && temp < 37.8) {

        color = yellow;
        text.innerHTML = "Alerta: Temperatura ALTA, abrir 50% das janelas laterais.";
        value_temp = -1;
    }
    else if (temp >= 37.8 && temp < 39.6) {

        color = yellow;
        text.innerHTML = "Alerta: Temperatura ALTA, abrir 100% das janelas laterais";
        value_temp = -1;

    }
    else if (temp >= 39.6) {

        color = red;
        text.innerHTML = "Alerta: Temperatura ALTA, abrir janelas zenitais e laterais.";
        value_temp = 1;

    }

    //Enviando os dados para o front
    bar.style.backgroundColor = color;
    temperatureValue.innerHTML = `${temp}°C`
    tempCardBar.style.backgroundColor = color;


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
function humidityProgressBar(lastHumidity) {

    let humidityBar = document.querySelector('.humidity-bar');
    let text = document.querySelector('.humidity-data .recommendation');
    let humidityValue = document.querySelector('.u-progress-bar p');
    let humCardBar = document.querySelector('.humidity_color');


    //Essa variável pega o valor real (último lido) de temperatura, enviado pela função 
    let humi = lastHumidity;


    /////////////CASOS DE TESTE UMIDADE://///////////
    // Para testar, comente a variável let humi = lastHumidity; e descomente a que você deseja testar
    //let humi = 50; //umi baixa
    //let humi = 90; //umi dentro do ideal
    //let humi = 96; //umi alta


    //Definindo as cores
    let color;
    let red = '#c74a34';
    let green = '#99cc66';


    //Fazendo as condições
    if (humi <= 73.5) {

        color = red;
        text.innerHTML = "Alerta: Umidade Relativa BAIXA.";
        value_hum = 1;

    }
    else if (humi > 73.5 && humi < 95) {

        color = green;
        text.innerHTML = "Alerta: Umidade Relativa dentro do ideal.";
        value_hum = 0;

    }
    else if (humi >= 95) {

        color = red;
        text.innerHTML = "Alerta: Umidade Relativa ALTA.";
        value_hum = 1;

    }

    //Enviando os dados para o front
    humidityValue.innerHTML = `${humi}%`
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


// Função para atualizar o horário da última medição na descrição da estufa/dispositivo
function showDate(lastDate) {
    let updateDate = document.querySelector('.hour');
    updateDate.innerHTML = lastDate;

}

//Função que abre o aba de descrição da estufa 
function StoveDesc(id) {

    let desc = document.querySelector('.right-side-column');
    console.log(id);    
    document.getElementById(`${id}`).style.boxShadow = "2px 2px 2px #000";
    desc.style.display = 'flex';
    stoveID = id;

}


//Função que irá esconder o botão de enviar confirmação de abertura quando a umi e temp estiverem dentro do padrão
function hideConfirmationButton(values){

    var div_button = document.querySelector('.confirm-div')

    if (values.temp == 0 && values.humid == 0){
        div_button.style.display = 'none'
    }
    else{
        div_button.style.display = 'flex'
    }

}


function sendMessage(lastDate,lastTemperature,lastHumidity){


}

//Função que fecha a aba de descrição da estufa
function closeDescPage() {

    var desc = document.querySelector('.right-side-column')
    desc.style.display = 'none';
    document.querySelector(`#${stoveID}`).style.boxShadow = 'none';

}


// Função para abrir o modal de adicionar uma nova estufa/dispositivo
function addStove() {

    let modal = document.querySelector('.modal-background');
    let container = document.querySelector('.container');
    let rightSideDiv = document.querySelector('.right-side-column');
    modal.style.display = 'flex';
    container.style.filter = 'blur(2px)';
    rightSideDiv.style.filter = 'blur(2px)';

}


// Fecha o modal de adicionar nova estufa/dispositivo
function closeCreateModal() {

    let modal = document.querySelector('.modal-background');
    let rightSideDiv = document.querySelector('.right-side-column');
    let container = document.querySelector('.container');
    modal.style.display = 'none';
    container.style.filter = 'none';
    rightSideDiv.style.filter = 'none';

    
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
function changeStoveTriangle(values){


    //Definindo cores
    let attention = "../images/attention_triangle.svg"; //amarelo
    let warning = "../images/warning_triangle.svg" //vermelho
    let color;

    let card_triangle = document.querySelector('.stove .caution');
    let desc_triangle = document.querySelector('.desc-triangle');

    //Fazendo condições
    if (values.temp == 1 || values.humid == 1){
        color = warning;

        card_triangle.setAttribute('src', color);
        desc_triangle.setAttribute('src', color);

    }
    else if (values.temp == -1){
        color = attention;

        card_triangle.setAttribute('src', color);
        desc_triangle.setAttribute('src', color);
    }
    else if (values.temp == 0 && values.humid == 0){
        card_triangle.style.display = 'none'
        desc_triangle.style.display = 'none'

    }

}

// Função para o caso de uma estufa inativa - front não recebe os dados
// Ainda em desenvolvimento
function makeInactive(lastTemperature,lastHumidity){
    if (lastTemperature == undefined || lastHumidity == undefined){
        console.log("inativo");
    }

    console.log(lastHumidity);
}

