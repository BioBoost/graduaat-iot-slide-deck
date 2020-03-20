# Teacher Notes

## Chapter 06 - Node-RED

Use docker to run Node-RED:

```bash
docker run -it -p 1880:1880 --name mynodered nodered/node-red
```

Start container after reboot or something:

```bash
docker start -ai /mynodered
```

Start container and remove on exit:

```bash
docker run -it --rm -p 1880:1880 nodered/node-red

```