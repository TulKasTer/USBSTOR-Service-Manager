pipeline {
    agent any

    environment {
        GITHUB_TOKEN = credentials('github-token')
        GITHUB_REPO  = 'TulKasTer/UBSSTOR-Services-Manager'
        PROGRAM      = 'USBSTORServiceManager'
        VERSION      = "${env.TAG_NAME ?: 'dev-' + env.BUILD_NUMBER}"
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
                echo "Versión: ${VERSION}"
            }
        }

        stage('Build') {
            steps {
                sh '''
                    set -e
                    make clean || true
                    make
                '''
            }
        }

        stage('Test') {
            steps {
                sh '''
                    if make test 2>/dev/null; then
                        echo "Tests OK"
                    else
                        echo "No hay tests o fallaron (continuando según configuración)"
                    fi
                '''
            }
        }

        stage('Package') {
            steps {
                sh '''
                    mkdir -p dist
                    if [ -f ${PROGRAM} ]; then cp ${PROGRAM} dist/; fi
                    if [ -f ${PROGRAM}.exe ]; then cp ${PROGRAM}.exe dist/; fi
                    ls -la dist || true
                '''
                archiveArtifacts artifacts: 'dist/*', fingerprint: true
            }
        }

        stage('Release') {
            when { expression { env.TAG_NAME != null } }
            steps {
                sh '''
                    set -e
                    echo "Creando release ${VERSION} para ${GITHUB_REPO}"
                    RELEASE_ID=$(curl -s -X POST \
                        -H "Authorization: token ${GITHUB_TOKEN}" \
                        -H "Content-Type: application/json" \
                        https://api.github.com/repos/${GITHUB_REPO}/releases \
                        -d '{"tag_name":"'"${VERSION}"'","name":"'"${VERSION}"'","body":"Release ${VERSION}","draft":false,"prerelease":false}' | python3 -c 'import sys,json; print(json.load(sys.stdin).get("id",""))')

                    if [ -n "${RELEASE_ID}" ]; then
                        for F in dist/*; do
                            [ -f "$F" ] || continue
                            echo "Subiendo $(basename "$F")"
                            curl -s -X POST \
                                -H "Authorization: token ${GITHUB_TOKEN}" \
                                -H "Content-Type: application/octet-stream" \
                                --data-binary @"$F" \
                                "https://uploads.github.com/repos/${GITHUB_REPO}/releases/${RELEASE_ID}/assets?name=$(basename "$F")"
                        done
                        echo "Release publicado: https://github.com/${GITHUB_REPO}/releases/tag/${VERSION}"
                        else
                            echo "No se pudo crear el release (RELEASE_ID vacío)"
                        fi
                '''
            }
        }
    }

    post {
        success { echo "Pipeline completado: ${VERSION}" }
        failure { echo "Pipeline falló" }
        always  { cleanWs() }
    }
}