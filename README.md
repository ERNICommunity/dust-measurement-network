[![Build Status](https://dev.azure.com/nej0372/ERNI%20Dust%20Measurement%20Network/_apis/build/status/ERNICommunity.hh-dust-fe-be?branchName=master)](https://dev.azure.com/nej0372/ERNI%20Dust%20Measurement%20Network/_build/latest?definitionId=1&branchName=master)

# ERNI Hack&amp;Hike - Dust Measurement Network: Frontend + DataSync

This repo contains 2 main projects: Frontend (Angular 11 application with ASP.NET 5.0 backend) and DataSyncService (.NET 5.0 console app). There is also shared Model project (EF Core model).

## Frontend

This is a web UI for dust measurement solution.
To run it you need to have [Node.js](https://nodejs.org) and [.NET Core](https://www.microsoft.com/net/download) installed on your computer. Once you have those:
 1. change into your clone directory `cd <your-clone-folder>`
 1. install .NET Core backend dependencies `dotnet restore`
 1. change to Angular app directory `cd Frontend/ClientApp`
 1. install Angular frontend dependencies `npm install`
 1. go back to Frontend `cd ..`
 1. launch the project by `dotnet run`
 1. navigate your browser to https://localhost:5001
 
This project uses PostgreSQL database, therefore you need to set connection string `DustDatabase` to the database instance you want to use. For local development it's recomended to use [ASP.NET Core Secret manager](https://docs.microsoft.com/en-us/aspnet/core/security/app-secrets?view=aspnetcore-2.1&tabs=windows#secret-manager). To do so, execute (inside Frontend project folder): `dotnet user-secrets set "ConnectionStrings:DustDatabase" "<your-connection-string>"`

## DataSync

This is a [.NET Core](https://www.microsoft.com/net/download) console application that takes data from InfluxDB (where sensors write), aggregates it into SQL database and enhances the data with additional weather information. 

## VS Code

The project is developed in [Visual Studio Code](https://code.visualstudio.com/) so if you use this IDE, your life is simpler, because VS Code project settings are already part of the repo. To launch a project, just hit F5 (pick what project you want to run in VS Code's Debug tab).

Few useful VS Code extensions:
- C# by Microsoft (mandatory for running/debugging .NET code)
- Angular Language Service by Angular
- TSlint by Microsoft
- EditorConfig for VS Code by EditorConfig

Remember that **whenever you pull new version, it's a good idea to reinstall backend and frontend dependencies**, as someone might have changed them.

**IMPORTANT**

Please **DON'T** commit connection strings/passwords/API keys etc. into repository, because its public. Preferrably use [ASP.NET Core Secret manager](https://docs.microsoft.com/en-us/aspnet/core/security/app-secrets?view=aspnetcore-2.1&tabs=windows#secret-manager) to store them locally.
