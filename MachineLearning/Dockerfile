FROM tiangolo/uwsgi-nginx-flask:python3.6 as base

RUN pip install --upgrade pip
RUN pip uninstall numpy
RUN pip install tensorflow keras scikit-learn 

FROM base 

ENV LISTEN_PORT 5001
EXPOSE 5001

COPY start_server.py /app/main.py
COPY models/best_model.h5 /app/models/best_model.h5
COPY models/preparation/ /app/models/preparation
WORKDIR /app
