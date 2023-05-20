import subprocess

fecha = subprocess.getoutput("date")
uptime = subprocess.getoutput("uptime")[0:18]
CPU = subprocess.getoutput("lscpu -e=CPU")
memoria = subprocess.getoutput("vmstat -s")
disco = subprocess.getoutput("df -h --output=source,fstype,size,used,avail,pcent,target -x tmpfs -x devtmpfs")


CPUL = list(CPU)
cantidadCPU = str(int(CPUL[-2] + CPUL[-1]) + 1)

memoriaLista = memoria.split(" ")

memoriaTotal = ""
memoriaUsada = ""
memoriaLibre = ""

for i in range(len(memoriaLista)):
    if(memoriaLista[i] == "total"):
        memoriaTotal = str(round(float(memoriaLista[i-2])/1000000))
    if(memoriaLista[i] == "memory\n" and memoriaLista[i-1] == "used"):
        memoriaUsada = str(round(float(memoriaLista[i-3])/1000))
    if(memoriaLista[i] == "memory\n" and memoriaLista[i-1] == "free"):
        memoriaLibre = str(round(float(memoriaLista[i-3])/1000))
    
discoLista = disco.split(" ")

discosEspaciosTotalesLista = []
discosEspaciosTotales = ""
discosEspaciosUsados = ""
discosEspaciosLibres = ""

cantidadDiscos = 0

discoLista = [x for x in discoLista if x != '']

for i in range(len(discoLista)):
    if("/dev" in discoLista[i]):
        discosEspaciosTotales = discosEspaciosTotales + discoLista[i + 2] + " "
        discosEspaciosUsados = discosEspaciosUsados + discoLista[i + 3] + " "
        discosEspaciosLibres = discosEspaciosLibres + discoLista[i + 4] + " "
        
        cantidadDiscos += 1


cantidadDiscos = str(cantidadDiscos)


text = """ 
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Información del sistema</title>
</head>
<body>

    <h1>Información del sistema</h1>
    
    <div>
        <table>
            <thead style="background-color:#A1A1A1;color:#FFFFFF">
                <tr>
                    <th>Parámetro</th>
                    <th>Valor</th>
                </tr>
            </thead>
            <tbody>    
                <tr>
                    <td>Fecha</td>
                    <td>""" + fecha + """</td>
                </tr>  
                <tr>
                    <td>Hora de inicio</td>
                    <td>""" + uptime + """</td>
                </tr>  
                <tr>
                    <td>Cantidad de CPUs/td>
                    <td>""" + cantidadCPU + """</td>
                </tr> 
                <tr>
                    <td>Memoria Total</td>
                    <td>""" + memoriaTotal + """ G</td>
                </tr>  
                <tr>
                    <td>Memoria usada</td>
                    <td>""" + memoriaUsada + """ M</td>
                </tr>  
                <tr>
                    <td>Memoria disponible</td>
                    <td>""" + memoriaLibre + """ M</td>
                </tr>  
                <tr>
                    <td>Cantidad de discos duros</td>
                    <td>""" + cantidadDiscos + """</td>
                </tr>  
                <tr>
                    <td>Tamaño total de cada disco</td>
                    <td>""" + discosEspaciosTotales + """</td>
                </tr>  
                <tr>
                    <td>Tamaño usado de cada disco</td>
                    <td>""" + discosEspaciosUsados + """</td>
                </tr>  
                <tr>
                    <td>Tamaño disponible de cada disco</td>
                    <td>""" + discosEspaciosLibres + """</td>
                </tr>  
            </tbody>
        </table>
    </div>
                        
</body>
</html>


"""

with open('info.html', 'w') as f:
    f.write(text)








