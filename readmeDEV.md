<h2><b>
<ul>

<li> Формат родительского конфигурационного файла:


    Config {
        type : "Parent"
        path : "Path to the parent path"

        Child {
            1 : "Path to the child element"
            ...
        }
    }

    Data {

    }
<li> Формат дочернего конфигурационного файла:


    Config {
        type : "Child"
        path : "Path to the parent path"
    }

</ul>
</b></h2>