pipeline {
    agent any

    environment {
        GITHUB_TOKEN    = credentials('github-token')
        GITHUB_REPO     = 'TulKasTer/UBSSTOR-Services-Manager'
        PROGRAM        = 'USBSTORServiceManager'
        VERSION         = "${env.TAG_NAME ?: 'dev-' + env.BUILD_NUMBER}"
    }

    stages {

        // ─────────────────────────────────────────────
        // 1. CHECKOUT
        // ─────────────────────────────────────────────
        stage('Checkout') {
            steps {
                checkout scm
                echo "Version: ${VERSION}"
            }
        }

        // ─────────────────────────────────────────────
        // 2. BUILD
        // ─────────────────────────────────────────────
        stage('Build') {
            steps {
                bat '''
                    echo Building...
                    mingw32-make clean
                    mingw32-make
                '''
            }
        }

        // ─────────────────────────────────────────────
        // 3. TEST
        // ─────────────────────────────────────────────
        stage('Test') {
            steps {
                bat '''
                    echo Running tests...
                    mingw32-make test
                '''
            }
            post {
                failure {
                    echo 'Tests failed. Release will not be published.'
                }
            }
        }

        // ─────────────────────────────────────────────
        // 4. PACKAGE
        // ─────────────────────────────────────────────
        stage('Package') {
            steps {
                bat '''
                    if not exist dist mkdir dist
                    copy %PROGRAM%.exe dist\\%PROGRAM%-windows-amd64.exe
                    dir dist\\
                '''
                archiveArtifacts artifacts: 'dist/*', fingerprint: true
            }
        }

        // ─────────────────────────────────────────────
        // 5. GITHUB RELEASE
        //    Only runs when triggered by a tag push
        // ─────────────────────────────────────────────
        stage('GitHub Release') {
            when {
                expression { env.TAG_NAME != null }
            }
            steps {
                bat '''
                    echo Creating GitHub release: %VERSION%

                    curl -s -X POST ^
                      -H "Authorization: token %GITHUB_TOKEN%" ^
                      -H "Content-Type: application/json" ^
                      https://api.github.com/repos/%GITHUB_REPO%/releases ^
                      -d "{\\"tag_name\\": \\"%VERSION%\\", \\"name\\": \\"Release %VERSION%\\", \\"body\\": \\"Automated release by Jenkins\\", \\"draft\\": false, \\"prerelease\\": false}" ^
                      -o release_response.json

                    for /f %%i in ('powershell -Command "(Get-Content release_response.json | ConvertFrom-Json).id"') do set RELEASE_ID=%%i
                    echo Release ID: %RELEASE_ID%

                    curl -s -X POST ^
                      -H "Authorization: token %GITHUB_TOKEN%" ^
                      -H "Content-Type: application/octet-stream" ^
                      --data-binary @dist\\%PROGRAM%-windows-amd64.exe ^
                      "https://uploads.github.com/repos/%GITHUB_REPO%/releases/%RELEASE_ID%/assets?name=%PROGRAM%-windows-amd64.exe"

                    echo Release published successfully!
                '''
            }
        }
    }

    post {
        success {
            echo "Pipeline completed successfully. Version: ${VERSION}"
        }
        failure {
            echo "Pipeline failed at one of the stages."
        }
        always {
            cleanWs()
        }
    }
}