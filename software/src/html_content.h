    #ifndef HTML_CONTENT_H
    #define HTML_CONTENT_H

    const char* index_html PROGMEM = "<!DOCTYPE html><html><head>    <script>        function init_function() {            document.getElementById('network_type').value = '{{network_type}}';            document.getElementById('eth_auto').value = '{{eth_auto}}';            document.getElementById('ap_auto').value = '{{ap_auto}}';            document.getElementById('wifi_auto').value = '{{wifi_auto}}';            document.getElementById('led_type').value = '{{led_type}}';            document.getElementById('num_outputs').value = parseInt('{{num_outputs}}');            document.getElementById('use_artsync').value = '{{use_artsync}}';            updateNetworkOptions();            updateLedOptions();        }        function updateNetworkOptions() {            console.log(\"running updateNetworkOptions\");            var networkType = document.getElementById(\"network_type\");            var ethSettings = document.getElementById(\"eth_settings\");            var apSettings = document.getElementById(\"ap_settings\");            var wifiSettings = document.getElementById(\"wifi_settings\");            if (networkType.value === \"ETHERNET\") {                ethSettings.style.display = \"\";                apSettings.style.display = \"none\";                wifiSettings.style.display = \"none\";            }            else if (networkType.value === \"ACCESS_POINT\") {                ethSettings.style.display = \"none\";                apSettings.style.display = \"\";                wifiSettings.style.display = \"none\";            }            else if (networkType.value === \"WIFI\") {                ethSettings.style.display = \"none\";                apSettings.style.display = \"none\";                wifiSettings.style.display = \"\";            }            updateNetworkAuto();        }        function updateNetworkAuto() {            console.log(\"running updateNetworkOptions\");            var ethAuto = document.getElementById(\"eth_auto\");            var ethManual = document.getElementById(\"eth_manual\");            var apAuto = document.getElementById(\"ap_auto\");            var apManual = document.getElementById(\"ap_manual\");            var wifiAuto = document.getElementById(\"wifi_auto\");            var wifiManual = document.getElementById(\"wifi_manual\");            if (ethAuto.value === \"false\") { ethManual.style.display = \"\"; }            else { ethManual.style.display = \"none\"; }            if (apAuto.value === \"false\") { apManual.style.display = \"\"; }            else { apManual.style.display = \"none\"; }            if (wifiAuto.value === \"false\") { wifiManual.style.display = \"\"; }            else { wifiManual.style.display = \"none\"; }        }        function updateLedOptions() {            console.log(\"running updateLedOptions\");            var ledTypeSelect = document.getElementById(\"led_type\");            var numOutputSelect = document.getElementById(\"num_outputs\");            var numOutputSelectContainer = document.getElementById(\"num_output_select_container\");            numOutputSelect.innerHTML = \"\";            var upperNum = 1;            if (ledTypeSelect.value === \"WS2815\") {                upperNum = 7;                numOutputSelectContainer.style.display = \"\";            }            else {                numOutputSelectContainer.style.display = \"none\";            }            for (let i = 0; i <= upperNum; i++) {                var thisOption = document.createElement(\"option\");                thisOption.value = i + 1;                thisOption.text = i + 1;                numOutputSelect.add(thisOption);            }            document.getElementById('num_outputs').value = parseInt('{{num_outputs}}');            updateNumLeds();        }        function updateNumLeds() {            console.log(\"running updateNumLeds\");            var numOutputSelect = document.getElementById(\"num_outputs\");            var outputLedsContainer = document.getElementById(\"output_leds_container\");            var children = outputLedsContainer.children;            for (var i = 0; i < children.length; i++) {                var child = children[i];                if (i < numOutputSelect.value) {                    child.style.display = \"\";                }                else {                    child.style.display = \"none\";                }            }        }    </script></head><body onload=\"init_function()\">    <form action='/submit' method='post'>        <h1>LATERAL_LEDS_CONTROLLER</h1>        <img src=\"info_labels.svg\" alt=\"Diagram showing board connections\" class=\"diagram\">        <h2>Network Settings</h2>        Controller IP Address: <input type='text' name='current_ip' value='{{current_ip}}' readonly><br>        Controller Network Mode Selection        <select id='network_type' name='network_type' onchange=\"updateNetworkOptions()\">            <option value='ETHERNET'>ETHERNET</option>            <option value='ACCESS_POINT'>ACCESS_POINT</option>            <option value='WIFI'>WIFI</option>        </select>        <br>        <br>        <div id=\"eth_settings\">            Auto-Assign Ethernet Connection            <select id='eth_auto' name='eth_auto' onchange=\"updateNetworkAuto()\">                <option value='true'>true</option>                <option value='false'>false</option>            </select>            <br>            <br>            <div id=\"eth_manual\">                Eth Static IP: <input type='text' name='eth_ip' value='{{eth_ip}}'><br>                Eth Subnet Mask: <input type='text' name='eth_sn' value='{{eth_sn}}'><br>                ETH Gateway: <input type='text' name='eth_gw' value='{{eth_gw}}'><br>            </div>        </div>        <div id=\"ap_settings\">            network name (ssid): <input type='text' name='ap_ssid' value='{{ap_ssid}}' minlength='4'><br>            network password: <input type='text' name='ap_pw' value='{{ap_pw}}' minlength='8'><br>            <br>            <br>            auto assign access point connection            <select id='ap_auto' name='ap_auto' onchange=\"updateNetworkAuto()\">                <option value='true'>true</option>                <option value='false'>false</option>            </select>            <br>            <br>            <div id=\"ap_manual\">                AP Static IP: <input type='text' name='ap_ip' value='{{ap_ip}}'><br>                AP Subnet Mask: <input type='text' name='ap_sn' value='{{ap_sn}}'><br>                AP Gateway: <input type='text' name='ap_gw' value='{{ap_gw}}'><br>            </div>        </div>        <div id=\"wifi_settings\">            network name (ssid): <input type='text' name='wifi_ssid' value='{{wifi_ssid}}'><br>            network password: <input type='text' name='wifi_pw' value='{{wifi_pw}}'><br>            <br>            <br>            auto assign wifi connection            <select id='wifi_auto' name='wifi_auto' onchange=\"updateNetworkAuto()\">                <option value='true'>true</option>                <option value='false'>false</option>            </select>            <br>            <br>            <div id=\"wifi_manual\">                Wifi Static IP: <input type='text' name='wifi_ip' value='{{wifi_ip}}'><br>                Wifi Subnet Mask: <input type='text' name='wifi_sn' value='{{wifi_sn}}'><br>                Wifi Gateway: <input type='text' name='wifi_gw' value='{{wifi_gw}}'><br>            </div>        </div>        <h2>LED SETTINGS</h2>        <img src=\"info_pcb.svg\" alt=\"Diagram showing the physical board\" class=\"diagram\">        Select your LED strip model.        <p class=\"note\"> WS281x family LEDs, when running 200+ pixels on a single line will begin to suffer framerate            bottlenecks.</p>        <p class=\"note\"> APA102 family strips need to be connected to the first two data lines but there is no            bottleneck and no need            to split them across multiple data lines. </p>        LED Type:        <select id='led_type' name='led_type' onchange=\"updateLedOptions()\">            <option value='WS2815'>WS2815</option>            <option value='APA102'>APA102</option>        </select>        <br>        <br>        <div id=\"num_output_select_container\">            Active Data Lines:            <select id='num_outputs' name='num_outputs' onchange=\"updateNumLeds()\">                <option value='1'>1</option>                <!-- dynamically insert more options here -->            </select>            <br>            <br>        </div>        <div id=\"output_leds_container\">            <div id=\"num_leds0_option\">d_0 line pixel count: <input type='number' name='num_leds0'                    value='{{num_leds0}}'><br>            </div>            <div id=\"num_leds1_option\">d_1 line pixel count: <input type='number' name='num_leds1'                    value='{{num_leds1}}'><br>            </div>            <div id=\"num_leds2_option\">d_2 line pixel count: <input type='number' name='num_leds2'                    value='{{num_leds2}}'><br>            </div>            <div id=\"num_leds3_option\">d_3 line pixel count: <input type='number' name='num_leds3'                    value='{{num_leds3}}'><br>            </div>            <div id=\"num_leds4_option\">d_4 line pixel count: <input type='number' name='num_leds4'                    value='{{num_leds4}}'><br>            </div>            <div id=\"num_leds5_option\">d_5 line pixel count: <input type='number' name='num_leds5'                    value='{{num_leds5}}'><br>            </div>            <div id=\"num_leds6_option\">d_6 line pixel count: <input type='number' name='num_leds6'                    value='{{num_leds6}}'><br>            </div>            <div id=\"num_leds7_option\">d_7 line pixel count: <input type='number' name='num_leds7'                    value='{{num_leds7}}'><br>            </div>        </div>        <br>        <br>        milliseconds until frame update: <input type='number' name='frame_time' value='{{frame_time}}'><br>        update frame on artsync packet: <select id='use_artsync' name='use_artsync'>            <option value='true'>true</option>            <option value='false'>false</option>        </select>        <br>        <br>        <input type='submit' value='Submit'>    </form></body><style>    body {        background-color: rgb(6, 1, 14);        color: rgb(255, 255, 255);        /* font-family: \"Courier New\", Courier, monospace; */        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;        font-weight: bold;        text-align: left;    }    h1 {        text-align: center;        color: #2CFF00;    }    form {        margin: 20px auto;        /* Center the form horizontally */        padding: 20px;        /* border: 1px solid white; */        max-width: 30em;        /* Set a maximum width for the form */    }    .note {        font-weight: lighter;    }    .radio_selector {        margin-top: 0px;        vertical-align: middle;        width: 100%;    }    input,    select {        width: 100%;        padding: 8px;        margin-top: 6px;        background-color: whitesmoke;        margin-bottom: 10px;        box-sizing: border-box;        /* Ensures padding is included in width calculation */    }    input[type=\"submit\"] {        background-color: white;        color: black;        border: none;        padding: 10px 20px;        margin-top: 6px;        font-size: 18px;        cursor: pointer;        font-weight: bold;    }    input[type=\"text\"] {        font-style: italic;    }    .diagram {        width: 30em;    }</style></html>";

    #endif
    