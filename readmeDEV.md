<h2><b>
<ul>

<li> Название конфигурационного файла: ConnectorConfig.jccf (json connector configuration file)
<li> Стандарт родительского конфигурационного файла:


    "Child": {
        "0": [
            path : <Path to child folder>
        ]
        ...
    },
    "DateOfLastChange": {
        "0": [
            path: path to .jccf config
            date: date #time_since_epoch
        ]
        ...
    },
    "path": <Path to parent folder>,
    "type": "Parent"

<li> Стандарт дочернего конфигурационного файла:


    "path": <Path to parent folder>,
    "type": "Child"

</ul>
</b></h2>